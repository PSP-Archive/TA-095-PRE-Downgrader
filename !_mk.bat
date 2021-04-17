PATH = c:/pspsdk/bin

cd idsprx
make clean
make
psp-build-exports -s exports.exp
rm *.o *.elf

cd ..
bin2c idsprx/ids.prx ids_prx.h ids_prx
make clean
make
rm *.o *.elf *.prx *.sfo ids_prx.h

cd idsprx
rm *.o *.S *.prx

pause