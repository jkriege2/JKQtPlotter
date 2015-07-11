NAME=xits
VERSION=1.108

SRC=sources
WEB=webfonts
DOC=documentation
TOOLS=tools
DOCSRC=$(DOC)/$(DOC)-$(SRC)
DIST=$(NAME)-$(VERSION)

PY=python
PY3=python3
SFNTTOOL=sfnttool
BUILD=$(TOOLS)/build.py
POSTPROCESS=$(TOOLS)/postprocess.py
COVERAGE=$(TOOLS)/fontcoverage.py

FONTS=math mathbold regular bold italic bolditalic
DOCS=user-guide xits-specimen
FEA=xits.fea

SFD=$(FONTS:%=$(SRC)/$(NAME)-%.sfd)
OTF=$(FONTS:%=$(NAME)-%.otf)
WOF=$(FONTS:%=$(WEB)/$(NAME)-%.woff)
EOT=$(FONTS:%=$(WEB)/$(NAME)-%.eot)
TEX=$(DOCS:%=$(DOCSRC)/%.tex)
PDF=$(DOCS:%=$(DOC)/%.pdf)

all: otf web

otf: $(OTF)
# sfntool does not support buiding EOT from OTF fonts
#web: $(WOF) $(EOT)
web: $(WOF)

xits-math.otf: $(SRC)/xits-math.sfd Makefile $(BUILD) $(POSTPROCESS)
	@echo "Building $@"
	@$(PY) $(BUILD) $< $@ $(VERSION)
	@$(PY) $(POSTPROCESS) $@ $@.post
	@mv $@.post $@

xits-mathbold.otf: $(SRC)/xits-mathbold.sfd Makefile $(BUILD) $(POSTPROCESS)
	@echo "Building $@"
	@$(PY) $(BUILD) $< $@ $(VERSION)
	@$(PY) $(POSTPROCESS) $@ $@.post
	@mv $@.post $@

%.otf: $(SRC)/%.sfd Makefile $(SRC)/$(FEA) $(BUILD) $(POSTPROCESS)
	@echo "Building $@"
	@$(PY) $(BUILD) $< $@ $(VERSION) $(SRC)/$(FEA)
	@$(PY) $(POSTPROCESS) $@ $@.post
	@mv $@.post $@

$(WEB)/%.woff: %.otf
	@echo "Building $@"
	@mkdir -p $(WEB)
	@$(SFNTTOOL) -w $< $@

$(WEB)/%.eot: %.otf
	@echo "Building $@"
	@mkdir -p $(WEB)
	@$(SFNTTOOL) -e -x $< $@

doc: $(PDF)

$(DOC)/%.pdf: $(DOCSRC)/%.tex
	@echo "Building $@"
	@context --nonstopmode --result=$@ $< 1>/dev/null

FONTLOG.txt: FONTLOG.txt.in $(COVERAGE) $(OTF)
	@echo "Generating $@"
	@$(PY3) $(COVERAGE) tools/Blocks.txt $< $(OTF) $@

dist: $(OTF) $(PDF) FONTLOG.txt
	@echo "Making dist tarball"
	@mkdir -p $(DIST)/$(SRC)
	@mkdir -p $(DIST)/$(DOC)
	@mkdir -p $(DIST)/$(DOCSRC)
	@mkdir -p $(DIST)/$(TOOLS)
	@cp $(SFD) $(DIST)/$(SRC)
	@cp $(SRC)/$(FEA) $(DIST)/$(SRC)
	@cp $(OTF) $(DIST)
	@cp $(POSTPROCESS) $(BUILD) $(COVERAGE) $(DIST)/$(TOOLS)
	@cp -r $(PDF) $(DIST)/$(DOC)
	@cp -r $(TEX) $(DIST)/$(DOCSRC)
	@cp -r Makefile OFL-FAQ.txt OFL.txt FONTLOG.txt tex $(DIST)
	@cp README.md $(DIST)/README.txt
	@zip -r $(DIST).zip $(DIST)

clean:
	@rm -rf $(OTF) $(DIST) $(DIST).zip
