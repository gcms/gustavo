CC=gcc
CFLAGS=-Wall -fbounds-checking -g `pkg-config gtk+-2.0 libart-2.0 libxml-2.0 --cflags`
LIBS=`pkg-config gtk+-2.0 libart-2.0 libxml-2.0 --libs` -lm

all: build
	$(CC) $(LIBS) contingoneline.o contingdrawing.o contingcomponent.o contingline.o contingutil.o continggroup.o contingtrans2.o contingtrans3.o contingbus.o contingmain.o contingxml.o contingdata.o contingitemdata.o contingfile.o continginfodialog.o contingsymbol.o continggen.o contingload.o contingfilecdf.o contingfilepeco.o contingbusbase.o contingserializable.o contingdrawingoperation.o contingdrawingoperationlabel.o contingvisitor.o contingvisitorcolor.o contingdrawingoperationdefault.o contingsysteminfo.o contingvisitorview.o contingloadflowfile.o contingmodel.o contingsettings.o contingstringpool.o contingstringfile.o contingloadflow.o contingconfig.o contingconfigdialog.o -o main

build: contingoneline.o contingdrawing.o contingcomponent.o contingline.o contingutil.o continggroup.o contingtrans2.o contingtrans3.o contingbus.o contingmain.o contingxml.o contingdata.o contingitemdata.o contingfile.o continginfodialog.o contingsymbol.o continggen.o contingload.o contingfilecdf.o contingfilepeco.o contingbusbase.o contingserializable.o contingdrawingoperation.o contingdrawingoperationlabel.o contingvisitor.o contingvisitorcolor.o contingdrawingoperationdefault.o contingsysteminfo.o contingvisitorview.o contingloadflowfile.o contingmodel.o contingsettings.o contingstringpool.o contingstringfile.o contingloadflow.o contingconfig.o contingconfigdialog.o

contingutil.o: contingutil.c
	$(CC) $(CFLAGS) -c contingutil.c -o contingutil.o

contingoneline.o: contingoneline.c
	$(CC) $(CFLAGS) -c contingoneline.c -o contingoneline.o

contingdrawing.o: contingdrawing.c
	$(CC) $(CFLAGS) -c contingdrawing.c -o contingdrawing.o

contingcomponent.o: contingcomponent.c
	$(CC) $(CFLAGS) -c contingcomponent.c -o contingcomponent.o

contingline.o: contingline.c
	$(CC) $(CFLAGS) -c contingline.c -o contingline.o

continggroup.o: continggroup.c
	$(CC) $(CFLAGS) -c continggroup.c -o continggroup.o

contingtrans2.o: contingtrans2.c
	$(CC) $(CFLAGS) -c contingtrans2.c -o contingtrans2.o

contingbus.o: contingbus.c
	$(CC) $(CFLAGS) -c contingbus.c -o contingbus.o

contingmain.o: contingmain.c
	$(CC) $(CFLAGS) -c contingmain.c -o contingmain.o

contingxml.o: contingxml.c
	$(CC) $(CFLAGS) -c contingxml.c -o contingxml.o

contingdata.o: contingdata.c
	$(CC) $(CFLAGS) -c contingdata.c -o contingdata.o

contingfile.o: contingfile.c
	$(CC) $(CFLAGS) -c contingfile.c -o contingfile.o
	
clean:
	rm -f contingoneline.o
	rm -f contingdrawing.o
	rm -f contingcomponent.o
	rm -f contingline.o
	rm -f contingutil.o
	rm -f continggroup.o
	rm -f contingtrans2.o
	rm -f contingtrans3.o
	rm -f contingbus.o
	rm -f contingmain.o
	rm -f contingxml.o
	rm -f contingfile.o
	rm -f contingfilecdf.o
	rm -f contingfilepeco.o
	rm -f contingdata.o
	rm -f contingitemdata.o
	rm -f continginfodialog.o
	rm -f contingsymbol.o
	rm -f continggen.o
	rm -f contingload.o
	rm -f contingbusbase.o
	rm -f contingserializable.o
	rm -f contingvisitor.o
	rm -f contingvisitorcolor.o
	rm -f contingdrawingoperationlabel.o
	rm -f contingdrawingoperationdefault.o
	rm -f contingsysteminfo.o
	rm -f contingvisitorview.o
	rm -f contingloadflowfile.o
	rm -f contingmodel.o
	rm -f contingsettings.o
	rm -f contingstringpool.o
	rm -f contingstringfile.o
	rm -f contingloadflow.o
	rm -f contingconfig.o
	rm -f contingconfigdialog.o
	rm -f main
