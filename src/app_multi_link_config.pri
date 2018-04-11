#--------------------------------------------------------------------------------
#app_multi_link_configure.pri

##Multi-link technology (Multi Link technology)
##Multi-link 技术，支持多链接库增删的一门工程管理技术。

#default sdk root is qqt-source/..
#user can modify this path in user_config_path()/app_configure.pri, then it will be included here.
#at qqt_library.pri, create_qqt_sdk and link_from_sdk will need this.
#this is different in every operating system
#qqt_library.pri need this configure

#2018年04月11日18:54:58
#这个文件用来加载用户电脑上的应用程序公共环境
#Windows:  C:\\Users\\xxx\\AppData\\Roaming\\qmake\\app_configure.pri
#Mac OS X: /Users/xxx/.qmake/app_configure.pri
#Ubuntu:   /home/xxx/.qmake/app_configure.pri
#公共路径：应用编译路径、LibrarySDK路径、产品输出路径

#--------------------------------------------------------------------------------
#这个pri依赖qqt_function.pri
#qqt_function.pri，哪里需要就在哪里包含。
include ($${PWD}/qqt_function.pri)

CONFIG_PATH =
CONFIG_FILE =

equals(QMAKE_HOST.os, Windows) {
    CONFIG_PATH = $$user_config_path()\\qmake
    CONFIG_FILE = $${CONFIG_PATH}\\app_configure.pri
} else {
    CONFIG_PATH = $$user_config_path()/.qmake
    CONFIG_FILE = $${CONFIG_PATH}/app_configure.pri
}
message($${TARGET} config file: $${CONFIG_FILE})

!exists($${CONFIG_FILE}) {
    mkdir("$${CONFIG_PATH}")
    empty_file($${CONFIG_FILE})
    ret = $$system(echo QQT_SDK_ROOT = > $${CONFIG_FILE})
    ret = $$system(echo QQT_BUILD_ROOT = >> $${CONFIG_FILE})
    ret = $$system(echo APP_DEPLOY_ROOT = >> $${CONFIG_FILE})
}

#your must config this file! following readme!
include ($${CONFIG_FILE})

#qqt build root, build station root
#link_from_build will need this path.
isEmpty(QQT_BUILD_ROOT)|isEmpty(QQT_SDK_ROOT) {
    message($${TARGET})
    message($${CONFIG_FILE})
    message("QQT_BUILD_ROOT = /user/setted/path is required, please modify qmake/app_configure.pri")
    message("QQT_SDK_ROOT = /user/setted/path is required")
    message("linux platform this pri is under app_link_qqt_library.pri")
    error("  please check $$CONFIG_FILE")
}
message(QQt build root: $$QQT_BUILD_ROOT)
message(QQt sdk root: $$QQT_SDK_ROOT)

