include prorab.mk

this_name := beerja

this_out_dir := build

this_srcs := $(call prorab-src-dir, .)

this_cxxflags += -Wall # enable all warnings
this_cxxflags += -Wnon-virtual-dtor # warn if base class has non-virtual destructor
this_cxxflags += -Werror # treat warnings as errors
this_cxxflags += -Wfatal-errors # stop on first error encountered
this_cxxflags += -fstrict-aliasing # in order to comply with the c++ standard more strictly
this_cxxflags += -g # include debugging symbols
this_cxxflags += -std=c++14

ifeq ($(debug),true)
    this_cxxflags += -DDEBUG
endif

this_ldlibs += -lmordavokne-opengl2 -lmorda -lpapki -lpuu -ljsondom -leasyhttp -lutki -lstdc++
this_ldlibs += -rdynamic

$(eval $(prorab-build-app))

define this_rules :=
run: $(prorab_this_name)
$(.RECIPEPREFIX)$(a)(cd $(d) && $(prorab_this_name))
endef
$(eval $(this_rules))
