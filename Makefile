default:
	make -C src

clean:
	rm -rf $(shell find . -name '*.o') $(shell find . -name '*.a') $(shell find . -name '*.bin') $(shell find . -name '*.iso')