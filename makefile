GXX=gcc
GXXFLAGS=-Wall
INSTDIR=/usr/local/bin

shprep: shadow_prep.o
	$(GXX) $(GXXFLAGS) shadow_prep.o -o shprep

shadow_prep.o: shadow_prep.c

clean:
	-rm shadow_prep.o

install:
	@if [ -d $(INSTDIR) ]; \
		then\
		cp shprep $(INSTDIR); \
		chmod a+x $(INSTDIR)/shprep; \
		chmod og-w $(INSTDIR)/shprep; \
		echo "shprep successfully installed in $(INSTDIR)."; \
	else \
		echo "could not install in $(INSTDIR). $(INSTDIR) does not exist."; \
	fi
