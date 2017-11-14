"""
    PCCC: A portable library for context-cognizant completions.
    Copyright (C) 2017  bcwarner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""
from ctypes import *
import time
import ctypes
import sublime
import sublime_plugin
import threading

"""
API Functions worth noting
	- view.get_regions(key)
	- view.substr(region)
	- view.file_name()
"""

PCCC_PATH="libpccc.so"
PCCC_BUFFER_DYNAMIC = 1

bufc = 0
pccc = CDLL(PCCC_PATH)

# This section is used to define how ctypes should interface with the library.
# Note that for all strings, we must use the string function encode to convert them to ASCII.

class pccc_context(Structure):
	_fields_ = [("buffers", c_void_p), # We use void_p here since we will not be directly accessing the symbol table
				("symbols", c_void_p),
				("threads", c_void_p)]

class pccc_suggestions(Structure):
	_fields_ = [("suggestions", POINTER(c_char_p)),
				("n", c_int)]

class pccc_buffer(Structure):
	_fields_ = [("contents", c_char_p),
				("name", c_char_p),
				("len", c_size_t),
				("flags", c_uint),
				("mutex", c_void_p)]

pccc.pccc_get_buffer.restype = POINTER(pccc_buffer)
pccc.pccc_add_new_buffer.restype = POINTER(pccc_buffer)
pccc.pccc_suggest_prefix.restype = POINTER(pccc_suggestions)

class PCCCEventListener(sublime_plugin.EventListener):
	def __init__(self):
		# When the program first starts up, we need to create a pccc_context so that PCCC can keep track of the files being edited.
		self.ctxt = pccc.pccc_init(None, 0)
		self.mutex = threading.Lock()
		self.modified = threading.Lock()
		print("Successfully initialized context with " + hex(self.ctxt))

	def on_modified_async(self, view):
		# Acquire the modifications mutex and the general mutex to ensure that calls to on_modified_async and other functions do not interfere with each other.
		# We sleep so that this function is not abused too often.
		time.sleep(1) 
		print("Acquiring modified mutexes.")
		if self.modified.locked() or self.modified.acquire(True, 2) == False: # Only lock if another modify thread is using it.
			return
		time.sleep(1)
		self.mutex.acquire() # Prevent query completions from interfering.

		print("Number of bufs: " + str(bufc))

		fn = view.file_name()
		buf = view.substr(sublime.Region(0, view.size()))

		if fn == None:
			return # If there is no filename, then there is no actual file being modified.

		print("Attempting to update buf " + fn)

		# Test if the file that the user edited does not exist.
		try:
			res = pccc.pccc_get_buffer(self.ctxt, fn.encode())
		except Exception as err:
			print(str(err))

		print("Successfully looked for buffer.")

		if not res: # If it doesn't exist, load the buffer (see on_load_async)
			print("Buffer does not exist.")
			self.on_load_async(view)
			self.mutex.release()
			self.modified.release()
			return
		if res.contents.len == 0: # Second conditional to achieve same as above.
			print("Buffer does not exist.")
			self.on_load_async(view)
			self.mutex.release()
			self.modified.release()
			return
		#else: # 
		#	print("Buffer contents for " + str(res.contents.name) + ": " + str(res.contents.contents))

		# If the buffer does exist, then we will update it.
		pccc.pccc_update_buffer(self.ctxt, fn.encode(), buf.encode(), view.size())
		print("Updated buffer '" + fn + "' of size " + str(view.size()))
		sleep(1)
		self.mutex.release()
		self.modified.release()
		

	def on_new_async(self, view):
		print("Created new buffer.")
		global bufc
		bufc += 1

	def on_load_async(self, view):
		# Load the outermost region.
		cbuf = view.substr(sublime.Region(0, view.size()))
		fn = view.file_name()

		global bufc
		bufc += 1
		print("Adding buffer " + str(fn))

		# Add the file using pccc_new_buffer.
		buf = pccc.pccc_add_new_buffer(self.ctxt, fn.encode(), cbuf.encode(), view.size(), PCCC_BUFFER_DYNAMIC)
		print("Added buffer '" + fn + "' of size " + str(view.size()))
		try:
			#print("Contents of buffer: ", buf.contents.contents)
			# Attempt to analyze the new buffer. This will collect all of the symbols in the file, which will then be available for suggestions.
			pccc.pccc_analyze(self.ctxt, buf)
		except Exception as err:
			print("Failed to analyze: " + str(err))
		print("Analyzed buffer.")
		
	def on_query_completions(self, view, prefix, locations):
		# Lock the mutex.
		print("=======Acquiring query completions mutex.=============")
		self.mutex.acquire()
		print("Finding suggestions for prefix '" + str(prefix) + "'.")
		
		# Using the function pccc_suggest_prefix, we can get a structure, pccc_suggestions, which contains an array of strings, and an amount n.

		r = pccc.pccc_suggest_prefix(self.ctxt, prefix.encode())
		# Return the results.
		print("Returned results")
		ret = []
		print(r.contents.suggestions[0])

		# We loop through all of the contents, and if not null, we return the suggestions to Sublime Text, where it will provide the suggestions to the user.
		for i in range(0, r.contents.n):
			if r.contents.suggestions[i] != None:
				print("Appending symbol: " + r.contents.suggestions[i].decode())
				ret.append([r.contents.suggestions[i].decode(), r.contents.suggestions[i].decode()])
		self.mutex.release()

		return ret
	