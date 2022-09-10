default:
	make -C src

clean_obj:
	rm -rf $(shell find . -name '*.o') $(shell find . -name '*.a')

clean:
	rm -rf $(shell find . -name '*.o') $(shell find . -name '*.a') $(shell find . -name '*.bin') $(shell find . -name '*.iso')