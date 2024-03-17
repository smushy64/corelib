# Description:  Corelib Build System
# Author:       Alicia Amarilla (smushyaa@gmail.com)
# File Created: January 30, 2024

CORE_LIB_VERSION_MAJOR=0
CORE_LIB_VERSION_MINOR=1
CORE_LIB_VERSION_PATCH=1

MAKEFLAGS += -s

ifndef $(COMPILER)
	COMPILER := clang
endif

ifeq ($(OS), Windows_NT)
	RUNNING_ON_WINDOWS := 1
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		RUNNING_ON_LINUX := 1
	else
		RUNNING_ON_MACOS := 1
	endif
endif

ifndef $(TARGET_PLATFORM)
	ifeq ($(RUNNING_ON_WINDOWS),1)
		TARGET_PLATFORM := win32
		ifneq '$(findstring ;,$(PATH))' ';'
			WIN32_SHELL := $(shell uname 2>/dev/null || echo Unknown)
			WIN32_SHELL := $(patsubst CYGWIN%,Cygwin,$(WIN32_SHELL))
			WIN32_SHELL := $(patsubst MSYS%,MSYS,$(WIN32_SHELL))
			WIN32_SHELL := $(patsubst MINGW%,MSYS,$(WIN32_SHELL))
			ifeq ($(WIN32_SHELL), Cygwin)
				WIN32_SHELL := _cygwin
			else
				ifeq ($(WIN32_SHELL), MSYS)
					WIN32_SHELL := _mingw
					MINGW := 1
				else
					WIN32_SHELL :=
				endif
			endif
		endif
	else
		ifeq ($(UNAME_S), Linux)
			TARGET_PLATFORM := linux
		else
			TARGET_PLATFORM := macos
		endif
	endif
	TARGET_PLATFORM_IS_CURRENT_PLATFORM := 1
endif

ifndef $(TARGET_ARCH)
	ifeq ($(TARGET_PLATFORM_IS_CURRENT_PLATFORM),)
		# TODO(alicia): more robust method of obtaining default arch target
		TARGET_ARCH := $(shell uname -m)
	else
		ifeq ($(TARGET_PLATFORM),win32)
			TARGET_ARCH := x86_64
		else
			ifeq ($(TARGET_PLATFORM),linux)
				TARGET_ARCH := x86_64
			else
				TARGET_ARCH := arm64
			endif
		endif
	endif
endif

TARGET_PLATFORM_VALID := $(filter win32 linux macos ios android, $(TARGET_PLATFORM))
TARGET_ARCH_VALID     := $(filter x86 x86_64 arm arm64, $(TARGET_ARCH))
COMPILER_VALID        := $(filter clang gcc msvc, $(COMPILER))

ifeq ($(COMPILER_VALID),)
	ERROR_MSG := $(error $(COMPILER) is not a supported compiler!)
endif

ifeq ($(TARGET_ARCH_VALID),)
	ERROR_MSG := $(error $(TARGET_ARCH) is not a supported target architecture!)
endif

ifeq ($(TARGET_PLATFORM_VALID),)
	ERROR_MSG := $(error $(TARGET_PLATFORM) is not a supported target platform!)
else
	INVALID_PLATFORM_ARCH := $(TARGET_PLATFORM) with $(TARGET_ARCH) is not supported!
	ifeq ($(TARGET_PLATFORM),win32)
		ifeq ($(TARGET_ARCH),arm)
			ERROR_MSG := $(error $(INVALID_PLATFORM_ARCH))
		endif
	else
		ifeq ($(TARGET_PLATFORM),linux)
			ifeq ($(TARGET_ARCH),arm)
				ERROR_MSG := $(error $(INVALID_PLATFORM_ARCH))
			endif
		else
			ifeq ($(TARGET_PLATFORM),android)
				TARGET_ARCH_VALID := $(filter arm arm64, $(TARGET_ARCH))
				ifeq ($(TARGET_ARCH_VALID),)
					ERROR_MSG := $(error $(INVALID_PLATFORM_ARCH))
				endif
			else
				# macos or ios
				TARGET_ARCH_VALID := $(filter arm64, $(TARGET_ARCH))
				ifeq ($(TARGET_ARCH_VALID),)
					ERROR_MSG := $(error $(INVALID_PLATFORM_ARCH))
				endif
			endif
		endif
	endif
endif

ifeq ($(COMPILER),msvc)
	ifneq ($(TARGET_PLATFORM),win32)
		ERROR_MSG := $(error msvc compiler is only supported for win32!)
	endif
	ifneq ($(CORE_ENABLE_STDLIB),1)
		ERROR_MSG := $(error msvc currently requires stdlib! enable it with CORE_ENABLE_STDLIB=1)
	endif
endif

OUTPUT_PATH := $(if $(OUTPUT_PATH_OVERRIDE),$(OUTPUT_PATH_OVERRIDE),./build)
OUTPUT_OBJ_PATH := $(OUTPUT_PATH)/obj

ifeq ($(SIMPLE_NAME),1)
	TARGET_NAME := libcore
else
	ifeq ($(NAME_OVERRIDE),)
		TARGET_NAME := libcore-$(TARGET_PLATFORM)-$(TARGET_ARCH)-$(if $(RELEASE),release,debug)
	else
		TARGET_NAME := $(NAME_OVERRIDE)
	endif
endif
ifeq ($(TARGET_PLATFORM),win32)
	TARGET := $(OUTPUT_PATH)/$(TARGET_NAME).dll
else
	TARGET := $(OUTPUT_PATH)/$(TARGET_NAME).so
endif

TEST_TARGET := $(OUTPUT_PATH)/test-$(TARGET_NAME)

ifeq ($(COMPILER),msvc)
TARGET      := $(subst /,\\,$(TARGET))
TEST_TARGET := $(subst /,\\,$(TEST_TARGET))
endif

all: $(TARGET) clean_build_junk
	@echo "Make: "$(TARGET_NAME)" compiled successfully!"

clean_build_junk: $(TARGET)
	@rm $(SOURCES_FILE)

test: $(TEST_TARGET)
	@echo "Make: running tests . . ."
	cd $(OUTPUT_PATH); $(TEST_TARGET)

print_info:
	@echo "Make: Configuration:"
	@echo "Make:    Compiler: "$(COMPILER)
	@echo "Make:    Standard: C11"
	@echo "Make:    Target:   "$(TARGET_PLATFORM)$(WIN32_SHELL)_$(TARGET_ARCH)
	@echo "Make:    Output:   "$(TARGET)

recurse = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call recurse,$d/,$2))

COMPILER_PATH := $(COMPILER)
ifeq ($(COMPILER),msvc)
	COMPILER_PATH := cl
endif

# check if compiler is in path
ifeq (, $(shell which $(COMPILER_PATH) 2> /dev/null))
	ERROR_MSG := $(error $(COMPILER_PATH) is not available! make sure it's installed/is in path!)
endif

STD_VERSION := -std=c11
ifeq ($(COMPILER),msvc)
	STD_VERSION := -std:c11
endif

SOURCES      := $(call recurse,impl,*.c)
SOURCES_FILE := $(OUTPUT_OBJ_PATH)/temp.c

CFLAGS        :=
CPPFLAGS      := CORE_ENABLE_EXPORT 
LDFLAGS       :=
INCLUDE       :=
OUTPUT_FILE   :=
WARNING_FLAGS :=

CPPFLAGS += $(if $(CORE_ENABLE_STDLIB),CORE_ENABLE_STDLIB,)
CPPFLAGS += $(if $(CORE_ENABLE_LOGGING),CORE_ENABLE_LOGGING,)
ifeq ($(TARGET_ARCH),x86_64)
	CPPFLAGS += CORE_ENABLE_SSE_INSTRUCTIONS
else
	CPPFLAGS += $(if $(CORE_ENABLE_SSE_INSTRUCTIONS),CORE_ENABLE_SSE_INSTRUCTIONS,)
endif
CPPFLAGS += $(if $(CORE_ENABLE_AVX_INSTRUCTIONS),CORE_ENABLE_AVX_INSTRUCTIONS,)
ifeq ($(TARGET_ARCH),arm64)
	CPPFLAGS += CORE_ENABLE_NEON_INSTRUCTIONS
else
	CPPFLAGS += $(if $(CORE_ENABLE_NEON_INSTRUCTIONS),CORE_ENABLE_NEON_INSTRUCTIONS,)
endif
CPPFLAGS += $(if $(CORE_ENABLE_ASSERTIONS),CORE_ENABLE_ASSERTIONS,)
CPPFLAGS += $(if $(CORE_ENABLE_DEBUG_BREAK),CORE_ENABLE_DEBUG_BREAK,)
CPPFLAGS += $(if $(CORE_ENABLE_DEBUG_ASSERTIONS),CORE_ENABLE_DEBUG_ASSERTIONS,)

DEFINED := $(CPPFLAGS)

CPPFLAGS += CORE_LIB_VERSION_MAJOR=$(CORE_LIB_VERSION_MAJOR)
CPPFLAGS += CORE_LIB_VERSION_MINOR=$(CORE_LIB_VERSION_MINOR)
CPPFLAGS += CORE_LIB_VERSION_PATCH=$(CORE_LIB_VERSION_PATCH)

TEST_CPPFLAGS := $(subst CORE_ENABLE_EXPORT,,$(CPPFLAGS))
INCLUDEFLAGS  := -I.
ifeq ($(COMPILER),msvc)
	# TODO(alicia): finish msvc flags!
	SOURCES_FILE := $(subst /,\\,$(SOURCES_FILE))
	INCLUDEFLAGS += -external:W0 -external:env:INCLUDE

	CFLAGS += -nologo
	CFLAGS += $(if $(OPTIMIZED),-O2 -fp:fast,-Od) -Oi
	CFLAGS += $(if $(NO_SYMBOLS),,-Z7)
	CFLAGS += -FC -wd4201 -wd4141 -wd4311 -Gm- -Gs- -Gs9999999 -Zc:preprocessor
	# -W4 -WX 

	LDFLAGS += -link -dll -nologo -debug:full -opt:ref
	LDFLAGS += -stack:0x100000,0x100000
	LDFLAGS += $(if $(CORE_ENABLE_STDLIB),,-nodefaultlib)

	ifeq ($(TARGET_PLATFORM),win32)
		LDFLAGS += kernel32.lib -subsystem:windows
	endif

	OUTPUT_FILE      := -Fe$(TARGET)
	TEST_OUTPUT_FILE := -Fe$(TEST_TARGET)

	DEF      := $(addprefix -D,$(filter-out CORE_ENABLE_SSE_INSTRUCTIONS,$(CPPFLAGS)))
	CPPFLAGS := $(DEF)
else
	OUTPUT_FILE      := -o $(TARGET)
	TEST_OUTPUT_FILE := -o $(TEST_TARGET)
	CFLAGS += $(if $(OPTIMIZED),-O2 -ffast-math,-O0)

	ifeq ($(MINGW),1)
		CFLAGS += $(if $(NO_SYMBOLS),,-g -gcodeview)
	else
		CFLAGS += $(if $(NO_SYMBOLS),,-g)
	endif

	WARNING_FLAGS += -Wall -Wextra -Werror -Werror=vla
	CFLAGS   += $(WARNING_FLAGS)
	LDFLAGS  += -shared
	LDFLAGS  += $(if $(CORE_ENABLE_STDLIB),,-nostdlib)

	ifeq ($(COMPILER),clang)
		LDFLAGS += -mno-stack-arg-probe
	endif

	ifeq ($(MINGW),1)
		LDFLAGS += -fuse-ld=lld $(if $(NO_SYMBOLS),,-Wl,//debug)
		LDFLAGS += -Wl,//stack:0x100000

		ifeq ($(TARGET_PLATFORM),win32)
			LDFLAGS += -lkernel32
		endif
	else
		CFLAGS   += -fPIC
		LDFLAGS  += -Wl,-z,stack-size=0x100000
		CPPFLAGS += _GNU_SOURCE
	endif

	TEST_CPPFLAGS := $(addprefix -D,$(TEST_CPPFLAGS))
	DEF           := $(addprefix -D,$(CPPFLAGS))
	CPPFLAGS      := $(DEF) -DCORE_COMMAND_LINE=\""$(COMPILER_PATH) $(STD_VERSION) $(SOURCES_FILE) $(COUT) $(INCLUDEFLAGS) $(CFLAGS) $(DEF)"\"
endif

DISPLAY_COMMAND_LINE := $(COMPILER_PATH) $(STD_VERSION) $(SOURCES_FILE) $(OUTPUT_FILE) $(INCLUDEFLAGS) $(CFLAGS) $(DEF) $(LDFLAGS)
COMMAND_LINE         := $(COMPILER_PATH) $(STD_VERSION) $(SOURCES_FILE) $(OUTPUT_FILE) $(INCLUDEFLAGS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

TEST_FILTER_CFLAGS   := -fPIC
TEST_FILTER_LDFLAGS  := -shared -nostdlib
TEST_COMMAND_LINE := $(COMPILER_PATH) $(STD_VERSION)\
	./test/test.c $(TEST_OUTPUT_FILE) $(INCLUDEFLAGS)\
	$(filter-out $(TEST_FILTER_CFLAGS),$(CFLAGS))\
	$(TEST_CPPFLAGS)\
	$(filter-out $(TEST_FILTER_LDFLAGS),$(LDFLAGS))\
	-L./build -l$(subst lib,,$(TARGET_NAME))

$(TARGET): print_info $(OUTPUT_OBJ_PATH) $(SOURCES_FILE)
	@echo "Make: "$(DISPLAY_COMMAND_LINE)
	$(COMMAND_LINE)

$(TEST_TARGET): $(TARGET)
	@echo "Make: "$(TEST_COMMAND_LINE)
	$(TEST_COMMAND_LINE)

$(OUTPUT_OBJ_PATH): 
	@echo "Make: creating output path '$(OUTPUT_PATH)' . . ."
	@mkdir $(OUTPUT_OBJ_PATH) -p

$(SOURCES_FILE): $(OUTPUT_OBJ_PATH) $(SOURCES)
	@echo "Make: generating sources file . . ."
	@echo "/* Generated on: "$(shell date)" */" > $(SOURCES_FILE)
	@echo "" >> $(SOURCES_FILE)
	for i in $(SOURCES); do echo "#include \""$$i"\"" >> $(SOURCES_FILE); done

build_win32_x86:
	@$(MAKE) TARGET_PLATFORM="win32" TARGET_ARCH="x86_64" RELEASE=1 OPTIMIZED=1 CORE_ENABLE_SSE_INSTRUCTIONS=1

build_linux_x86:
	@$(MAKE) TARGET_PLATFORM="linux" TARGET_ARCH="x86_64" RELEASE=1 OPTIMIZED=1 CORE_ENABLE_SSE_INSTRUCTIONS=1

build_macos_arm:
	@$(MAKE) TARGET_PLATFORM="macos" TARGET_ARCH="arm64" RELEASE=1 OPTIMIZED=1 CORE_ENABLE_NEON_INSTRUCTIONS=1

DOXYFILE_DEFAULT := $(file <./docs/Doxyfile_default)
DOXYFILE_LOCAL   := Doxyfile_$(TARGET_PLATFORM)_$(TARGET_ARCH)
DOXYFILE         := ./docs/$(DOXYFILE_LOCAL)

DOXYFILE_SETTINGS := PREDEFINED += $(DEFINED)

ifeq ($(COMPILER),clang)
	DOXYFILE_SETTINGS += __clang__
endif
ifeq ($(COMPILER),gcc)
	DOXYFILE_SETTINGS += __GNUC__
endif
ifeq ($(COMPILER),msvc)
	DOXYFILE_SETTINGS += _MSC_VER
endif
ifeq ($(TARGET_PLATFORM),win32)
	DOXYFILE_SETTINGS += _WIN32
endif
ifeq ($(TARGET_PLATFORM),linux)
	DOXYFILE_SETTINGS += __linux__
endif
ifeq ($(TARGET_PLATFORM),macos)
	DOXYFILE_SETTINGS += __APPLE__ TARGET_OS_MAC
endif
ifeq ($(TARGET_PLATFORM),android)
	DOXYFILE_SETTINGS += __ANDROID__
endif
ifeq ($(TARGET_PLATFORM),ios)
	DOXYFILE_SETTINGS += __APPLE__ TARGET_OS_IPHONE
endif
ifeq ($(TARGET_ARCH),x86)
	DOXYFILE_SETTINGS += __i386__
	ifeq ($(MINGW),1)
		DOXYFILE_SETTINGS += __MINGW32__
	endif
endif
ifeq ($(TARGET_ARCH),x86_64)
	DOXYFILE_SETTINGS += __x86_64__
	ifeq ($(MINGW),1)
		DOXYFILE_SETTINGS += __MINGW64__
	endif
endif
ifeq ($(TARGET_ARCH),arm)
	DOXYFILE_SETTINGS += __arm__
	ifeq ($(MINGW),1)
		DOXYFILE_SETTINGS += __MINGW32__
	endif
endif
ifeq ($(TARGET_ARCH),arm64)
	DOXYFILE_SETTINGS += __aarch64__
	ifeq ($(MINGW),1)
		DOXYFILE_SETTINGS += __MINGW64__
	endif
endif

define DOXYFILE_APPEND
$(DOXYFILE_SETTINGS)
PROJECT_NUMBER = $(CORE_LIB_VERSION_MAJOR).$(CORE_LIB_VERSION_MINOR).$(CORE_LIB_VERSION_PATCH)
endef

ifeq ($(RUNNING_ON_WINDOWS),1)
	ifneq (,$(shell which pwsh 2> /dev/null))
		OPEN_DOCS    := pwsh -Command Invoke-Expression ./docs/html/index.html
		DOCS_BROWSER := "default browser"
	endif
else
	ifeq (,$(shell which firefox 2> /dev/null))
		ifneq (,$(shell which google-chrome 2> /dev/null))
			OPEN_DOCS    := google-chrome ./docs/html/index.html
			DOCS_BROWSER := "google chrome"
		endif
	else
		OPEN_DOCS    := firefox ./docs/html/index.html
		DOCS_BROWSER := "firefox"
	endif
endif

docs:
	$(if $(shell which doxygen 2> /dev/null),,$(error doxygen is not available! make sure it's installed/is in path before generating docs!))
	@echo "Make: generating documentation . . ."
	$(file >$(DOXYFILE),$(DOXYFILE_DEFAULT)$(DOXYFILE_APPEND))
	@cd ./docs; doxygen $(DOXYFILE_LOCAL) -q
	@echo "Make: documentation generated at path ./docs/html/index.html"
	$(if $(OPEN_DOCS),echo "Make: opening documentation in $(DOCS_BROWSER) . . .",)
	$(OPEN_DOCS)

LSP_FLAGS := $(COMPILER_PATH) $(STD_VERSION) $(subst -I.,-I..,$(INCLUDEFLAGS)) $(DEF) -DCORE_COMMAND_LINE=\"\" -D_CLANGD $(WARNING_FLAGS) -nostdinc
lsp:
	@echo "Make: generating compile_flags.txt for clangd . . ."
	-@rm -f ./core/compile_flags.txt 2> /dev/null || true
	-@rm -f ./impl/compile_flags.txt 2> /dev/null || true
	for i in $(LSP_FLAGS);\
		do echo $$i >> ./core/compile_flags.txt;\
	done
	@cp ./core/compile_flags.txt ./impl/compile_flags.txt

help:
	@echo "Make recipes:"
	@echo "   all      compile project for current platform."
	@echo "   docs     run doxygen to generate documentation."
	@echo "            documentation generated takes compiler options into account."
	@echo "            windows:     if pwsh is in path, opens docs automatically in default browser"
	@echo "            linux/macos: if firefox or google-chrome is in path, opens docs automatically"
	@echo "   test     run tests to see if there are any errors (runs all first)."
	@echo "   lsp      generate compile_flags.txt"
	@echo "   help     print this message."
	@echo "   help_ex  print extended help message."
	@echo "   clean    clean output directory."
	@echo
	@echo "Compilation options for corelib:"
	@echo "   COMPILER=[compiler]             set compiler (default=clang)"
	@echo "                                   valid: clang, gcc, msvc"
	@echo "   TARGET_PLATFORM=[platform]      set target platform (default=current)"
	@echo "                                   valid: win32, linux, macos, ios, android"
	@echo "   TARGET_ARCH=[arch]              set target architecture (default=current)"
	@echo "                                   valid: x86(win32, linux), x86_64(win32, linux), arm(android), arm64(win32, linux, macos, ios, android)"
	@echo "   OUTPUT_PATH_OVERRIDE=[path]     override output path. (default=./build)"
	@echo "   NAME_OVERRIDE=[name]            override library output name. (default=libcore-platform-arch-release_mode)"
	@echo "   SIMPLE_NAME=1                   output name is simply libcore. (takes precedence over NAME_OVERRIDE)"
	@echo "   RELEASE=1                       build in release mode. does not actually do anything other than makes generated output path /release rather than /debug."
	@echo "   OPTIMIZED=1                     build with optimizations on (default=off)."
	@echo "   NO_SYMBOLS=1                    build without debug symbols (default=with symbols, .pdb on Windows, DWARF otherwise)."
	@echo "   CORE_ENABLE_STDLIB=1            enable stdlib (Windows does not build stdlib by default)."
	@echo "   CORE_ENABLE_LOGGING=1           enable logging (default=off)."
	@echo "   CORE_ENABLE_SSE_INSTRUCTIONS=1  enable SSE instructions (default=on, x86_64 only)"
	@echo "   CORE_ENABLE_AVX_INSTRUCTIONS=1  enable AVX instructions (default=off, x86_64 only)"
	@echo "   CORE_ENABLE_NEON_INSTRUCTIONS=1 enable NEON instructions (default=on, arm only)"
	@echo "   CORE_ENABLE_ASSERTIONS=1        enable assertions. see docs for more info. (default=off)"
	@echo "   CORE_ENABLE_DEBUG_BREAK=1       enable debug break. see docs for more info. (default=off)"
	@echo "   CORE_ENABLE_DEBUG_ASSERTIONS=1  enable debug assertions. see docs for more info. (default=off)"

help_ex:
	@echo "Additional make recipes:"
	@echo "   build_win32_x86  compile release mode for win32 x86_64."
	@echo "   build_linux_x86  compile release mode for linux x86_64."
	@echo "   build_macos_arm  compile release mode for macos arm64."
	@echo "   build_wasm64     compile release mode for wasm64."

clean:
	@echo "Make: cleaning output path "$(OUTPUT_PATH)" . . ."
	-@rm -f $(OUTPUT_PATH)/* 2> /dev/null || true
	@echo "Make: cleaning ./docs/html . . ."
	-@rm -f ./docs/html/* -r 2> /dev/null || true

err: ; $(ERROR_MSG)

.PHONY: all docs test lsp help help_ex clean build_win32_x86 build_linux_x86 build_macos_arm build_wasm64 err clean_build_junk spit

