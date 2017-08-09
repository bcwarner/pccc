import sublime
import sublime_plugin

class PCCCEventListener(sublime_plugin.EventListener):
	def on_modified_async(self, view):
		pass

	def on_load_async(self, view):
		pass

	def on_query_completions(self, view, prefix, locations):
		return []