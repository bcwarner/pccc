import sublime
import sublime_plugin

"""
API Functions worth noting
	- view.get_regions(key)
	- view.substr(region)
	
"""

class PCCCEventListener(sublime_plugin.EventListener):
	def on_modified_async(self, view):
		# Rescan the buffer.
		
		pass

	def on_load_async(self, view):
		pass

	def on_query_completions(self, view, prefix, locations):
		return []