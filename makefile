include prorab.mk

this_name := stocks

this_out_dir := build

this_srcs := $(call prorab-src-dir, .)

this_ldlibs += -lmordavokne-opengl2 -lmorda -lstdc++

$(eval $(prorab-build-app))
