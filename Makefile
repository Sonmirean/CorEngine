# CorEngine GNU Make build (Linux)

CXX        ?= c++
AR         ?= ar
ARFLAGS    := rcs
PKG_CONFIG ?= pkg-config

BUILD_DIR ?= build
TARGET    ?= $(BUILD_DIR)/libCorEngine.a
PLATFORM  ?= wayland

PREFIX     ?= /usr/local
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR     ?= $(PREFIX)/lib

# Set PACKAGE_PATH to additional pkg-config directories, for example:
#   make PACKAGE_PATH=/opt/vulkan/lib/pkgconfig
# Set VULKAN_SDK when using the LunarG SDK. Its include and lib directories
# are added without relying on a project-local dependency bundle.
PACKAGE_PATH ?=
VULKAN_SDK  ?=

ifneq ($(strip $(PACKAGE_PATH)),)
export PKG_CONFIG_PATH := $(PACKAGE_PATH)$(if $(PKG_CONFIG_PATH),:$(PKG_CONFIG_PATH))
endif

ifeq ($(PLATFORM),wayland)
PLATFORM_DEFINES  := -DCORENGINE_BUILDFOR_LINUX=1 -DCORENGINE_USE_PLATFORM_WAYLAND
PLATFORM_PACKAGES := wayland-client
else ifeq ($(PLATFORM),xlib)
PLATFORM_DEFINES  := -DCORENGINE_BUILDFOR_LINUX=2 -DCORENGINE_USE_PLATFORM_X11 -DCORENGINE_USE_PLATFORM_XLIB
PLATFORM_PACKAGES := x11
else
$(error Unsupported PLATFORM '$(PLATFORM)'; use PLATFORM=wayland or PLATFORM=xlib)
endif

ifeq ($(strip $(VULKAN_SDK)),)
REQUIRED_PACKAGES := vulkan $(PLATFORM_PACKAGES)
VULKAN_LDLIBS     :=
else
REQUIRED_PACKAGES := $(PLATFORM_PACKAGES)
VULKAN_LDLIBS     := -lvulkan
endif

PACKAGE_CPPFLAGS = $(shell $(PKG_CONFIG) --cflags $(REQUIRED_PACKAGES) 2>/dev/null)
PACKAGE_LDLIBS   = $(shell $(PKG_CONFIG) --libs $(REQUIRED_PACKAGES) 2>/dev/null) $(VULKAN_LDLIBS)
SDK_CPPFLAGS     = $(if $(strip $(VULKAN_SDK)),-I$(VULKAN_SDK)/include)
SDK_LDFLAGS      = $(if $(strip $(VULKAN_SDK)),-L$(VULKAN_SDK)/lib)

PROJECT_CPPFLAGS := -Iinclude $(PLATFORM_DEFINES) $(SDK_CPPFLAGS) $(PACKAGE_CPPFLAGS)
REQUIRED_CXXFLAGS := -std=c++20 -MMD -MP

CPPFLAGS ?=
CXXFLAGS ?= -O2 -Wall -Wextra
LDFLAGS  ?=
LDLIBS   ?=

SOURCES      := $(wildcard src/*.cpp)
OBJECTS      := $(patsubst src/%.cpp,$(BUILD_DIR)/obj/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

.PHONY: all clean install uninstall check-deps print-config

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/obj/%.o: src/%.cpp | check-deps
	@mkdir -p $(@D)
	$(CXX) $(PROJECT_CPPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(REQUIRED_CXXFLAGS) -c $< -o $@

check-deps:
	@command -v "$(PKG_CONFIG)" >/dev/null || { echo "error: pkg-config is required" >&2; exit 1; }
	@$(PKG_CONFIG) --exists $(REQUIRED_PACKAGES) || { \
		echo "error: missing Linux development package(s): $(REQUIRED_PACKAGES)" >&2; \
		echo "       set PKG_CONFIG_PATH or use PACKAGE_PATH=/path/to/pkgconfig" >&2; \
		exit 1; \
	}
	@printf '%s\n' '#include <vulkan/vk_enum_string_helper.h>' | \
		$(CXX) $(PROJECT_CPPFLAGS) $(CPPFLAGS) -x c++ -E -o /dev/null - 2>/dev/null || { \
		echo "error: vulkan/vk_enum_string_helper.h was not found" >&2; \
		echo "       install the Vulkan SDK headers or use VULKAN_SDK=/path/to/sdk" >&2; \
		exit 1; \
	}

install: $(TARGET)
	install -d "$(DESTDIR)$(LIBDIR)" "$(DESTDIR)$(INCLUDEDIR)"
	install -m 644 $(TARGET) "$(DESTDIR)$(LIBDIR)/libCorEngine.a"
	cp -R include/CorE "$(DESTDIR)$(INCLUDEDIR)/"

uninstall:
	rm -f "$(DESTDIR)$(LIBDIR)/libCorEngine.a"
	rm -rf "$(DESTDIR)$(INCLUDEDIR)/CorE"

clean:
	rm -rf "$(BUILD_DIR)"

print-config:
	@printf '%-20s %s\n' \
		'CXX' '$(CXX)' \
		'Linux backend' '$(PLATFORM)' \
		'pkg-config packages' '$(REQUIRED_PACKAGES)' \
		'PKG_CONFIG_PATH' '$(PKG_CONFIG_PATH)' \
		'VULKAN_SDK' '$(VULKAN_SDK)' \
		'compile flags' '$(PROJECT_CPPFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(REQUIRED_CXXFLAGS)' \
		'consumer link flags' '$(SDK_LDFLAGS) $(PACKAGE_LDLIBS) $(LDFLAGS) $(LDLIBS)'

-include $(DEPENDENCIES)
