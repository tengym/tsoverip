CS_STB_MAKE_VERSION :=0

export PROJECT_DIR = $(shell pwd)

SUBDIRS = porting midware app 

MAKEFUNC = @MakeSubDir() \
        { \
               if [ -f makefile -o -f Makefile ]; then \
                        echo ""; \
                        pwd; \
                        make $(1); \
                        if [ "$$?" != "0" ]; then \
                            exit 1; \
                        fi; \
                fi; \
        }; \

MAKEME = cd $(2); MakeSubDir $(1); cd ..;

LOOPMAKEFUNC = $(MAKEFUNC) $(foreach dir,$(SUBDIRS),$(call MAKEME,$(1),$(dir)))

all:
	$(call LOOPMAKEFUNC,)
clean:
	$(call LOOPMAKEFUNC,clean)


