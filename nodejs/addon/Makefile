all: rebuild test

rebuild:
	@node-gyp config rebuild

test:
	@mocha --reporter list

.PHONY: rebuild test
