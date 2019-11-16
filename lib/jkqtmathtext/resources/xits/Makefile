NAME=XITS
VERSION=1.302

SRC=sources
WEB=webfonts
TOOLS=tools
DIST=$(NAME)-$(VERSION)
DIST_CTAN=$(DIST)-CTAN

PY=python
MAKEFNT=$(TOOLS)/makefnt.py
MAKEWEB=$(TOOLS)/makeweb.py
NORMALIZE=$(TOOLS)/sfdnormalize.py

FONTS=$(NAME)Math-Regular $(NAME)Math-Bold \
      $(NAME)-Regular $(NAME)-Bold $(NAME)-Italic $(NAME)-BoldItalic

SFD=$(FONTS:%=$(SRC)/%.sfd)
OTF=$(FONTS:%=%.otf)
WOF=$(FONTS:%=$(WEB)/%.woff)
NRM=$(FONTS:%=$(SRC)/%.nrm)
CHK=$(FONTS:%=$(SRC)/%.chk)

all: otf

otf: $(OTF)
web: $(WOF)
normalize: $(NRM)
check: $(CHK)

%.otf: $(SRC)/%.sfd $(SRC)/%.fea
	@echo "Building $@"
	@$(PY) $(MAKEFNT) $< $@ --version=$(VERSION) --features=$(word 2,$+)

%.nrm: %.sfd $(NORMALIZE)
	@echo "Normalizing $(*F)"
	@$(PY) $(NORMALIZE) $< $@
	@if [ "`diff -u $< $@`" ]; then cp $@ $<; touch $@; fi

%.chk: %.sfd $(NORMALIZE)
	@echo "Normalizing $(*F)"
	@$(PY) $(NORMALIZE) $< $@
	@diff -u $< $@ || (rm -rf $@ && false)

$(WEB)/%.woff: %.otf
	@echo "Building $@"
	@mkdir -p $(WEB)
	@$(PY) $(MAKEWEB) $< $(WEB)

dist: check dist-ctan $(OTF) $(WOF)
	@echo "Making dist tarball"
	@mkdir -p $(DIST)/$(WEB)
	@cp $(OTF) $(DIST)
	@cp $(WOF) $(DIST)/$(WEB)
	@cp -r OFL.txt FONTLOG.txt $(DIST)
	@cp README.md $(DIST)/README.txt
	@zip -r $(DIST).zip $(DIST)

dist-ctan: $(OTF)
	@echo "Making CTAN dist tarball"
	@mkdir -p $(DIST_CTAN)
	@cp $(OTF) $(DIST_CTAN)
	@cp -r OFL.txt FONTLOG.txt $(DIST_CTAN)
	@cp README.md $(DIST_CTAN)/README.txt
	@zip -r $(DIST_CTAN).zip $(DIST_CTAN)

clean:
	@rm -rf $(OTF) $(DIST) $(DIST).zip $(DIST_CTAN) $(DIST_CTAN).zip $(NRM)\
		$(CHK)
