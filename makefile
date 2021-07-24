include prorab.mk

$(eval $(call prorab-config, config))

this_name := beerja

this_srcs := $(call prorab-src-dir, .)

# this is to prevent deprecation error in date library
this_cxxflags += -DHAS_UNCAUGHT_EXCEPTIONS=1

this_ldlibs += -lmordavokne-opengl -lmorda -lpapki -ltreeml -ljsondom -lhttpc -lutki -lm -lstdc++
this_ldlibs += -rdynamic

$(eval $(prorab-build-app))

define this_rules :=
run: $(prorab_this_name)
$(.RECIPEPREFIX)$(a)(cd $(d) && $(prorab_this_name))
endef
$(eval $(this_rules))
