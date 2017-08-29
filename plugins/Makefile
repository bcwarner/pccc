SUBLIME_DIR=/opt/sublime_text/Packages
SUBLIME_NAME=pccc.sublime-package
OUTPUT_EXTENSIONS=.sublime-package

all: sublime
	echo "Building all plugins."

sublime: pccc_sublime.py
	zip $(SUBLIME_NAME) pccc_sublime.py
	cp $(SUBLIME_NAME) $(SUBLIME_DIR)/$(SUBLIME_NAME)

clean:
	rm -f *$(OUTPUT_EXTENSIONS)
	rm -f $(SUBLIME_DIR)/$(SUBLIME_NAME)
