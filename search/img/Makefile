IMG_OBJECTS = saddleback-xx-yy maze maze-loop
DOT_OBJECTS = max-sum-invariant strstr kmp kmp-fallback kmp-prefix-func fp-strstr fallback-tr \
lazy-fallback-tr good-char good-char-2 good-char-3 good-char-4 bad-char-1 bad-char-2 \
good-suffix-case1-1 good-suffix-case2-1 good-suffix-case2-2
DOT_SOURCES = $(foreach file, $(DOT_OBJECTS), $(file).dot)

#suffix replacement, replace .dot with .png
PNG_OBJECTS = $(DOT_SOURCES:.dot=.png)
EPS_OBJECTS = $(foreach file, $(IMG_OBJECTS), $(file).eps)

.SUFFIXES: .eps .jpg .png .dot .ps

.PHONY: all

all: convert dot

convert: $(IMG_OBJECTS)

# for details, refer to: 
#    http://www.gnu.org/software/autoconf/manual/make/
# Makefile syntax, replace each '%' to 'eps
$(IMG_OBJECTS): % : %.eps

# pattern rule matching
# Makefile auto variable: $< first prerequisite, $@ target
%.eps : %.png
	convert $< $@

dot: $(DOT_OBJECTS)

$(DOT_OBJECTS): % : %.ps

# Makefile auto variable: $< first prerequisite, $@ target
%.ps : %.dot
	dot -Tps -o $@ $<

png: $(PNG_OBJECTS)

%.png : %.dot
	dot -Tpng -o $@ $<

clean:
	rm -f *.ps *~
	rm -f $(PNG_OBJECTS)
	rm -f $(EPS_OBJECTS)
