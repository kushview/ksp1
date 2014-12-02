#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, shutil, sys, platform
from waflib import TaskGen, Task, Build, Options, Utils, Errors
from waflib.TaskGen import taskgen_method, feature, after_method, before_method

def options (opts):
    opts.add_option('--apple-sdk', default='', \
        dest='apple_sdk', type='string', help="Specify the Apple SDK absolute path")
    opts.add_option('--apple-version-min', default='', \
        dest='apple_version_min', type='string', help="Specify the Apple minimum version target")
    opts.add_option('--apple-userdirs', default=False, \
        dest='apple_userdirs', action='store_true', help="Install binaries into User directories. e.g. $HOME/Applications for programs")

def configure (conf):
    conf.env.APPLE_USERDIRS = conf.options.apple_userdirs

    conf.env.APPLE_VERSION_MIN = conf.options.apple_version_min
    if conf.options.apple_version_min:
        conf.env.append_unique ("CFLAGS", ['-mmacosx-version-min='+conf.options.apple_version_min])
        conf.env.append_unique ("CXXFLAGS", ['-mmacosx-version-min='+conf.options.apple_version_min])
        conf.env.append_unique ("LINKFLAGS", ['-mmacosx-version-min='+conf.options.apple_version_min])

    conf.env.APPLE_SDK = conf.options.apple_sdk
    if conf.options.apple_sdk:
        conf.env.append_unique ("CFLAGS", ["-isysroot", conf.options.apple_sdk])
        conf.env.append_unique ("CXXFLAGS", ["-isysroot", conf.options.apple_sdk])
        conf.env.append_unique ("LINKFLAGS", ["-isysroot", conf.options.apple_sdk])

app_info = '''
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist SYSTEM "file://localhost/System/Library/DTDs/PropertyList.dtd">
<plist version="0.9">
<dict>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleGetInfoString</key>
    <string>Created by Waf</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>NOTE</key>
    <string>THIS IS A GENERATED FILE, DO NOT MODIFY</string>
    <key>CFBundleExecutable</key>
    <string>%s</string>
</dict>
</plist>
'''
"""
plist template
"""

def is_program (ctx):
    return 'cprogram' in ctx.features or 'cxxprogram' in ctx.features

def default_install_dir (ctx):
    if is_program (ctx):
        return "/Applications"
    else:
        return "/Library/Frameworks"

@taskgen_method
def is_mac_package(self):
    return self.env['MACBUNDLE'] or getattr (self, 'mac_bundle', False) or \
            self.env['MACPACKAGE'] or getattr(self, 'mac_package', False)

@feature('c', 'cxx')
def set_macosx_deployment_target (self):
    """
    see WAF issue 285 and also and also http://trac.macports.org/ticket/17059
    """
    if self.env['MACOSX_DEPLOYMENT_TARGET']:
        os.environ['MACOSX_DEPLOYMENT_TARGET'] = self.env['MACOSX_DEPLOYMENT_TARGET']
    elif 'MACOSX_DEPLOYMENT_TARGET' not in os.environ:
        if Utils.unversioned_sys_platform() == 'darwin':
            os.environ['MACOSX_DEPLOYMENT_TARGET'] = '.'.join(platform.mac_ver()[0].split('.')[:2])

@taskgen_method
def create_bundle_dirs (self, name, out):
    """
    Create bundle folders, used by :py:func:`create_task_macplist` and :py:func:`create_task_mac_package`
    """
    bld = self.bld
    dir = out.parent.find_or_declare(name)
    dir.mkdir()
    macos = dir.find_or_declare(['Contents', 'MacOS'])
    macos.mkdir()
    return dir

def bundle_name_for_output (out, ext='.app'):
    name = out.name
    k = name.rfind('.')
    if k >= 0:
        name = name[:k] + ext
    else:
        name = name + ext
    return name

@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method('apply_link')
def create_task_mac_package (self):
    """
    To compile an executable into a Mac application (a .app), set its *mac_app* attribute::

        def build(bld):
            bld.shlib(source='a.c', target='foo', mac_app = True)

    To force *all* executables to be transformed into Mac applications::

        def build(bld):
            bld.env.MACAPP = True
            bld.shlib(source='a.c', target='foo')
    """
    if self.env['MACPACKAGE'] or getattr(self, 'mac_package', False):
        out = self.link_task.outputs[0]

        pkgext = getattr (self, 'mac_extension', '.app')
        name = bundle_name_for_output(out, pkgext)
        dir = self.create_bundle_dirs (name, out)

        n1 = dir.find_or_declare(['Contents', 'MacOS', out.name])

        self.apptask = self.create_task ('macpkg', self.link_task.outputs, n1)
        inst_to = getattr (self, 'install_path', default_install_dir(self)) + '/%s/Contents/MacOS/' % name
        self.bld.install_files (inst_to, n1, chmod=Utils.O755)

        if getattr(self, 'mac_resources', None):
            res_dir = n1.parent.parent.make_node('Resources')
            inst_to = getattr(self, 'install_path', default_install_dir(self)) + '/%s/Resources' % name
            for x in self.to_list(self.mac_resources):
                node = self.path.find_node(x)
                if not node:
                    raise Errors.WafError('Missing mac_resource %r in %r' % (x, self))

                parent = node.parent
                if os.path.isdir(node.abspath()):
                    nodes = node.ant_glob('**')
                else:
                    nodes = [node]
                for node in nodes:
                    rel = node.path_from(parent)
                    tsk = self.create_task('macpkg', node, res_dir.make_node(rel))
                    self.bld.install_as(inst_to + '/%s' % rel, node)

        if getattr(self.bld, 'is_install', None):
            # disable the normal binary installation
            self.install_task.hasrun = Task.SKIP_ME

@feature ('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method ('apply_link')
def create_task_apple_plist (self):
    """
    Create a :py:class:`apple.macplist` instance.
    """
    if self.env['MACPACKAGE'] or getattr (self, 'mac_package', False):
        out = self.link_task.outputs[0]
        pkgext = getattr (self, 'mac_extension', '.app')
        name = bundle_name_for_output (out, pkgext)
        dir = self.create_bundle_dirs (name, out)
        n1 = dir.find_or_declare (['Contents', 'Info.plist'])
        self.plisttask = plisttask = self.create_task ('macplist', [], n1)

        if getattr (self, 'mac_plist', False):
            node = self.path.find_resource (self.mac_plist)
            if node:
                plisttask.inputs.append (node)
            else:
                plisttask.code = self.mac_plist
        else:
            plisttask.code = app_info % self.link_task.outputs[0].name

        inst_to = getattr(self, 'install_path', default_install_dir(self)) + '/%s/Contents/' % name
        self.bld.install_files(inst_to, n1)

@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method ('apply_link')
def create_task_appke_pkginfo (self):
    """
    Create a :py:class:`waflib.Tools.c_osx.macplist` instance.
    """
    if self.is_mac_package():
        out = self.link_task.outputs[0]
        pkgext = getattr (self, 'mac_extension', '.app')
        name = bundle_name_for_output (out, pkgext)

        dir = self.create_bundle_dirs(name, out)
        n1 = dir.find_or_declare (['Contents', 'PkgInfo'])
        self.pkginfotask = pkginfotask = self.create_task ('pkginfo', [], n1)

        if getattr (self, 'mac_pkginfo', False):
            node = self.path.find_resource (self.mac_pkginfo)
            if node:
                pkginfotask.inputs.append (node)
            else:
                pkginfotask.code = self.mac_pkginfo
        else:
            if is_program (self):
                pkginfotask.code = "APPL"
            else:
                pkginfotask.code = "BNDL"

        inst_to = getattr (self, 'install_path', default_install_dir (self)) + '/%s/Contents/' % name
        self.bld.install_files (inst_to, n1)

@feature ('cshlib', 'cxxshlib')
@before_method ('apply_link', 'propagate_uselib_vars')
def fix_library_soname (self):
    if self.is_mac_package():
        self.env['cshlib_PATTERN']   = self.env['cxxshlib_PATTERN'] = "%s" # remove any file extension
        use = self.use = self.to_list (getattr(self, 'use', []))

app_dirs = ['Contents', 'Contents/MacOS', 'Contents/Resources']

class macpkg (Task.Task):
    """
    Create mac packages
    """
    color = 'CYAN'
    def run(self):
        self.outputs[0].parent.mkdir()
        shutil.copy2(self.inputs[0].srcpath(), self.outputs[0].abspath())

class macplist (Task.Task):
    """
    Create plist files
    """
    color = 'CYAN'
    ext_in = ['.bin']
    def run(self):
        if getattr(self, 'code', None):
            txt = self.code
        else:
            txt = self.inputs[0].read()
        self.outputs[0].write(txt)

class pkginfo (Task.Task):
    """
    Create PkgInfo files
    """
    color = 'CYAN'
    ext_in = ['.bin']
    def run(self):
        if getattr (self, 'code', None):
            txt = self.code
        else:
            txt = self.inputs[0].read()
        self.outputs[0].write (txt)
