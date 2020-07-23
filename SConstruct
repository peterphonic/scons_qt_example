import os
import sys
import SCons.Script

env = SCons.Environment.Environment(MSVC_VERSION='14.1', TARGET_ARCH='x86')


qt_root = 'C:/svn/3rdParty/3rdPartyPackages/Qt-5.12.1_vs2017/5.12.1/msvc2017'
env['QTDIR'] = qt_root
env.Tool('qt')
print('originial QT_UICCOM:')
print(env['QT_UICCOM'])

env['ENV']['PKG_CONFIG_PATH'] = os.path.join(qt_root, 'lib', 'pkgconfig')


qt_inc_dir = [os.path.join(qt_root, 'include'),
              os.path.join(qt_root, 'include', 'QtCore'),
              os.path.join(qt_root, 'include', 'QtWidgets'),
              os.path.join(qt_root, 'include', 'QtGui')]




env.Append(CPPFLAGS=['-Od', '-D_DEBUG', '-RTC1', '-MDd'])
env.Append(LINKFLAGS=['/INCREMENTAL', '/DEBUG', '/NOLOGO'])
        


env.Append(CPPPATH=qt_inc_dir)
               
# Default scons implementation fo the Uic tool chain does that :
# [['$QT_UIC', '$QT_UICDECLFLAGS', '-o', '${TARGETS[0]}', '$SOURCE'], ['$QT_UIC', '$QT_UICIMPLFLAGS', '
# TARGETS[1]}', '$SOURCE'], ['$QT_MOC', '$QT_MOCFROMHFLAGS', '-o', '${TARGETS[2]}', '${TARGETS[0]}']]
# We only need the first action in this list of 3 actions.  So we override QT_UICCOM
env['QT_UICCOM'] = [['$QT_UIC', '$QT_UICDECLFLAGS', '-o', '${TARGETS[0]}', '$SOURCE']]
env['QT_UICDECLPREFIX'] = 'ui_'
print('QT_UICCOM I need:')
print(env['QT_UICCOM'])

SCons.Script.SConscript('#ASREngineSpawner/ASREngineSpawner.sc',
                        exports='env',
                        variant_dir='build/ASREngineSpawner',
                        duplicate=0)