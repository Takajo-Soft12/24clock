all:main.mot


main.mot: main
	h8300-elf-objcopy -O srec main main.mot

#�饤�֥�����������쵤�˼¹Բ�ǽ�ե��������
main:main.c subdirs
	h8300-elf-gcc -O2 -Wall -nostdlib -mh -mn \
         -Xlinker -T h8-3664.x -L./libs -I./libs \
	-o main main.c  ./libs/crt0.o -lutil -lc -lgcc

subdirs:
	(cd libs;make all)

# H8�ޥ�����ܡ��ɤ˥ץ�������ɤ���
install: main.mot 
	h8write -3664 main.mot /dev/cu.usbserial-*

./h8write.fast:h8write.fast.c
	gcc -Wall h8write.fast.c -o h8write.fast  2>/dev/null
# H8�ޥ�����ܡ��ɤ˥ץ�������ɤ���
install-fast: main.mot ./h8write.fast
	./h8write.fast -3664 main.mot /dev/cu.usbserial-*

# ������PS�ե�����κ���
createps:main.ps
main.ps:main.c
	a2ps-j -t -h -n -L \
	"%month %mday %year %hour:%min (`whoami` : `pwd | sed 's/\/.*\///'`)" \
	 main.c > main.ps

# PS�ե�����β���ɽ��
display:main.ps
	gv main.ps

# PS�ե�����ΰ���
printout:main.ps
	lpr main.ps

# �ƥ��֥������Ȥ���
clean:
	rm -f *.revas *.objdump *.nm *.hexdump *.lst *.o *~ 
	rm -f main main.s main.mot main.ps main.pdf main.log
	rm -f h8write.fast
	(cd libs;make clean)
