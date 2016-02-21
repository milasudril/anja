#!/usr/bin/python3

import xml.etree.ElementTree as ET
import sys

chapters=0
sections=0
subsection=0
subsubsections=0
listings=0

labels={}
float_name=''
float_type=''

bibrefs={}
bibrefs_reverse={}
citecounter=0

pass_counter=0

def printWrapper(string):
	if pass_counter > 0:
		print(string,end='')

def title(node):
	printWrapper('''<h1>''' + node.text + '''</h1>''')

def author(node):
	printWrapper('''<p class="titlepage">''' +node.text + '''</p>''')

def includegraphics(node):
	printWrapper('''<source srcset="''' + node.attrib["src"] + '''" media="''' + node.attrib["media"] +'''">''')

def titlepic(node):
	printWrapper('''<picture>''')
	for altgraphics in node.findall('includegraphics'):
		includegraphics(altgraphics)

	last=node.findall('includegraphics[last()]')[0]
	printWrapper('''<img src="''' + last.attrib["src"] + '">')
	printWrapper('''</picture>''')

def abstract(node):
	printWrapper('<div class="abstract">')
	processElements(node)
	printWrapper('</div>')

def tableofcontents(node,document):
	printWrapper('''<nav id="toc">
<h2>Table of contents</h2>
<ol>''')
	sibling_prev='chapter'
	for sibling in document.findall('*'):
		if sibling.tag=='chapter' or sibling.tag=='section'	\
			or sibling.tag=='subsection':
			if (sibling.tag=='chapter' and sibling_prev=='section') \
				or (sibling.tag=='section' and sibling_prev=='subsection'):
				printWrapper('''</ol>''')
			if sibling.tag=='chapter' and sibling_prev=='subsection':
				printWrapper('''</ol></ol>''')
			if (sibling_prev=='chapter' and sibling.tag=='section') \
				or (sibling_prev=='section' and sibling.tag=='subsection'):
				printWrapper('''<ol>''')
			printWrapper('''<li><a href="#''' + sibling.attrib["id"] + '''">''')
			if sibling.text != None:
				printWrapper(sibling.text)
			processElements(sibling)
			printWrapper('''</a></li>''')
			sibling_prev=sibling.tag

	if sibling_prev=='subsection':
		printWrapper('''</ol></ol>''')
	if sibling_prev=='section':
		printWrapper('''</ol>''')
	printWrapper('''</ol>
</nav>''')

def chapter(node):
	global chapters
	global sections
	global subsections
	global subsubsections
	global listings
	chapters=chapters+1
	printWrapper('<h2 id="' + node.attrib["id"] + '">' + str(chapters) \
		+ '<span class="fill"></span>')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</h2>')
	labels[ node.attrib["id"] ]={'Chapter',str(chapters)}
	sections=0
	subsections=0
	subsubsections=0
	listings=0

def section(node):
	global sections
	global subsections
	global subsubsections
	sections=sections + 1
	name=str(chapters)+'.'+str(sections)
	printWrapper('<h3 id="' + node.attrib["id"] + '">' + name \
		+ '<span class="fill"></span>')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</h3>')
	labels[ node.attrib["id"] ]=['Section',name]
	subsections=0
	subsubsections=0

def subsection(node):
	global subsections
	global subsubsections
	subsections=subsections + 1
	name=str(chapters)+'.'+str(sections)+'.'+str(subsections)
	printWrapper('<h4 id="' + node.attrib["id"] + '">' + name \
		+ '<span class="fill"></span>')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</h4>')
	labels[ node.attrib["id"] ]=['Section',name]
	subsubsections=0

def subsubsection(node):
	global subsubsections
	subsubsections=subsections + 1
	name=str(chapters)+'.'+str(sections)+'.'+str(subsections)+'.'+str(subsubsections)
	printWrapper('<h5 id="' + node.attrib["id"] + '">' + name \
		+ '<span class="fill"></span>')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</h5>')
	labels[ node.attrib["id"] ]=['Section',name]

def cite(node):
	global bibrefs
	global citecounter
	global bibrefs_reverse
	if not node.text in bibrefs:
		citecounter=citecounter + 1
		bibrefs[node.text] = citecounter
		bibrefs_reverse[citecounter]=node.text
	printWrapper('&#160;<cite><a href="#' + node.text +'">[' \
		+ str(bibrefs[node.text]) + ']</a></cite>' + node.tail)

def libname(node):
	printWrapper('<code>')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</code>')
	if node.tail != None:
		printWrapper(node.tail)

def quantity(node):
	printWrapper(node.text + '&#160;' + node.attrib["unit"] + ' ' + node.tail)

def listing(node):
	global listings
	global float_name
	global float_type
	global labels
	listings=listings + 1
	float_name=str(chapters)+'.'+str(listings)
	float_type='Listing'
	labels[node.attrib["id"]]=[float_type,float_name]
	printWrapper('''<div class="listing" id="''' + node.attrib["id"] + '''">''')
	processElements(node)
	printWrapper('''</div>''')

def caption(node):
	printWrapper('<p><strong>' + float_type + ' ' + float_name  + '</strong>: ' \
		+ node.text)
	processElements(node)
	printWrapper('''</p>''')

def code(node):
	printWrapper('''<pre><code class="''' + node.attrib["language"] + '''">'''\
		+ node.text\
		+ '''</code></pre>''')

def ref(node):
	if node.text in labels:
		printWrapper('<a href="#' + node.text + '">' + labels[node.text][0] + ' '\
			+labels[node.text][1]+'</a>')
	else:
		printWrapper('??')

	if node.tail!=None:
		printWrapper(node.tail)

def bibliography(node):
	printWrapper('<h2>References</h2>')
	for cite in sorted(bibrefs_reverse):
		for item in node.findall("./item[@id='" + bibrefs_reverse[cite] + "']"):
			printWrapper('<p class="bibitem" id="' + bibrefs_reverse[cite] + '">'\
				' [' + str(cite) + '] ' + node.text)
			processElements(item)
			printWrapper('</p>')

def url(node):
	printWrapper('<a href="' + node.text + '" target="blank">'+node.text+'</a>')

def defaultrule(node):
	printWrapper('''<''' + node.tag + '''>''')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('''</''' + node.tag + '''>''')
	if node.tail != None:
		printWrapper(node.tail)

def processElements(document):
	for node in document.findall('*'):
		if node.tag=='title':
			title(node)
		elif node.tag=='author':
			author(node)
		elif node.tag=='titlepic':
			titlepic(node)
		elif node.tag=='abstract':
			abstract(node)
		elif node.tag=='tableofcontents':
			tableofcontents(node,document)
		elif node.tag=='chapter':
			chapter(node)
		elif node.tag=='section':
			section(node)
		elif node.tag=='subsection':
			subsection(node)
		elif node.tag=='subsubsection':
			subsubsection(node)
		elif node.tag=='cite':
			cite(node)
		elif node.tag=='libname':
			libname(node)
		elif node.tag=='quantity':
			quantity(node)
		elif node.tag=='listing':
			listing(node)
		elif node.tag=='code':
			code(node)
		elif node.tag=='caption':
			caption(node)
		elif node.tag=='ref':
			ref(node)
		elif node.tag=='bibliography':
			bibliography(node)
		elif node.tag=='url':
			url(node)
		elif node.tag=='dirname':
			libname(node)
		elif node.tag=='prgname':
			libname(node)
		else:
			defaultrule(node)


def main(argv):
	tree=ET.parse(sys.stdin)
	document=tree.getroot()
	global chapters
	global sections
	global subsection
	global subsubsections
	global listings
	global pass_counter

	print('''<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>''' + document.findall('title')[0].text + '''</title>
<link rel="stylesheet" href="highlight/styles/default.css" type="text/css" />
<link rel="stylesheet" href="format.css" type="text/css" />
<script src="highlight/highlight.pack.js" type="text/javascript"></script>
<script type="text/javascript">hljs.initHighlightingOnLoad();</script>
</head>
<body>''')
	chapters=0
	sections=0
	subsections=0
	subsubsections=0
	listings=0

	processElements(document)

	chapters=0
	sections=0
	subsections=0
	subsubsections=0
	listings=0

	pass_counter=pass_counter + 1

	processElements(document)

	print('''</body>
</html>''')



if __name__ == "__main__":
	main(sys.argv[1:])
