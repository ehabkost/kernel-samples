all:
	$(MAKE) -C /usr/src/kernels/`uname -r` M=$(PWD)

%:
	$(MAKE) -C /usr/src/kernels/`uname -r` M=$(PWD) $@
