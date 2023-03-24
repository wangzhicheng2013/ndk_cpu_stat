LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := cpu_stat
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES := main.cpp

LOCAL_LDLIBS := -pthread
LOCAL_CFLAGS := -Wall -Werror -Wextra -Wno-unused-parameter -Wno-return-type -Wno-unused-variable -Wno-unused-function
include $(BUILD_EXECUTABLE)