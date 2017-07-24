#!/usr/bin/python3

import xml.etree.ElementTree as ET
import sys
import json
import html

in_dir=''

chapters=0
sections=0
subsection=0
subsubsections=0
listings=0
figures=0
tables=0
countmode=0

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
	printWrapper('<h1>' + node.text + '</h1>')

def subtitle(node):
	printWrapper('<sub-title>')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</sub-title>')
	if node.tail != None:
		printWrapper(node.tail)

def author(node):
	printWrapper('<p class="titlepage">' +node.text + '</p>')

def includegraphics(node):
	printWrapper('<source srcset="' + node.attrib["src"]+'"')
	if "media" in node.attrib:
		printWrapper(' media="' + node.attrib["media"]+'"')
	printWrapper('>')

def includegraphics2(node):
	printWrapper('<img src="' + node.attrib["src"]+'" alt="'\
		+ node.attrib["src"]+'">')
	if node.tail != None:
		printWrapper(node.tail)

def titlepic(node):
	printWrapper('''<div class="titlepic">
<picture>''')
	alts=list(node.findall('includegraphics'))
	for altgraphics in alts:
		includegraphics(altgraphics)
	printWrapper('<img src="' + alts[int( len(alts)/2 )].attrib["src"] + '" alt="Title picture">')
	printWrapper('''</picture>
</div>''')

def abstract(node):
	printWrapper('<div class="abstract">')
	processElements(node)
	printWrapper('</div>')

def tableofcontents(node,document):
	printWrapper('''<nav id="toc">
<h2>Table of contents</h2>
<ol><li>''')
	sibling_prev='none'
	chapter=0
	section=0
	subsection=0
	subsubsection=0
	global countmode
	for sibling in document.findall('*'):
		if sibling.tag=='appendix':
			chapter=0
			countmode=1

		if sibling.tag=='chapter':
			chapter=chapter+1
			section=0
			subsection=0
			subsubsection=0

		if sibling.tag=='section':
			section=section + 1
			subsection=0
			subsubsection=0

		if sibling.tag=='subsection':
			subsection=subsection + 1
			subsubsection=0

		if sibling.tag=='subsubsection':
			subsubsection=subsubsection + 1


		if sibling.tag=='chapter' or sibling.tag=='section'	\
			or sibling.tag=='subsection' or sibling.tag=='chapter-star':
			if ((sibling.tag=='chapter' or sibling.tag=='chapter-star') \
				and sibling_prev=='section') \
				or (sibling.tag=='section' and sibling_prev=='subsection'):
				printWrapper('</li></ol>')
			if (sibling.tag=='chapter' or sibling.tag=='chapter-star')\
				and sibling_prev=='subsection':
				printWrapper('</li></ol></li></ol>')

			if ( (sibling_prev=='chapter' or sibling_prev=='chapter-star') \
				and sibling.tag=='section') \
				or (sibling_prev=='section' and sibling.tag=='subsection'):
				printWrapper('<ol><li>')
			elif sibling_prev!='none':
				printWrapper('</li><li>')

			printWrapper('<a href="#' + sibling.attrib["id"] + '">')
			if sibling.tag=='chapter':
				printWrapper('<span class="fill">' + chapstr(chapter) + \
					'</span> ')
			if sibling.tag=='section':
				printWrapper('<span class="fill">' + chapstr(chapter) \
					+ '.' + str(section) + '</span> ')
			if sibling.tag=='subsection':
				printWrapper('<span class="fill">' + chapstr(chapter) \
					+ '.' + str(section) \
					+ '.' + str(subsection) + '</span> ')
			printWrapper('<span class="content">')
			if sibling.text != None:
				printWrapper(sibling.text)
			processElements(sibling)
			printWrapper('</span>')
			printWrapper('</a>')
			sibling_prev=sibling.tag

	if sibling_prev=='subsection':
		printWrapper('</li></ol></li></ol>')
	if sibling_prev=='section':
		printWrapper('</li></ol>')
	printWrapper('''</ol>
</nav>''')
	countmode=0

def chapter(node):
	global chapters
	global sections
	global subsections
	global subsubsections
	global listings
	global figures
	global countmode
	global tables
	chapters=chapters+1
	printWrapper('<h2 id="' + node.attrib["id"] + '"><span class="fill">' \
		+ chapstr(chapters) + '</span> ')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</h2>')
	if countmode:
		labels[ node.attrib["id"] ]=['Appendix',chapstr(chapters)]
	else:
		labels[ node.attrib["id"] ]=['Chapter',chapstr(chapters)]
	sections=0
	subsections=0
	subsubsections=0
	listings=0
	figures=0
	tables=0

def chapter_star(node):
	global sections
	global subsections
	global subsubsections
	global listings
	global figures
	global countmode
	global tables
	global chapters

	printWrapper('<h2 id="' + node.attrib["id"] + '">')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</h2>')
	sections=0
	subsections=0
	subsubsections=0
	listings=0
	figures=0
	tables=0

def section(node):
	global sections
	global subsections
	global subsubsections
	sections=sections + 1
	name=chapstr(chapters)+'.'+str(sections)
	printWrapper('<h3 id="' + node.attrib["id"] + '"><span class="fill">' \
		+ name + '</span>')
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
	name=chapstr(chapters)+'.'+str(sections)+'.'+str(subsections)
	printWrapper('<h4 id="' + node.attrib["id"] + '"><span class="fill">' \
		+ name + '</span>')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</h4>')
	labels[ node.attrib["id"] ]=['Section',name]
	subsubsections=0

def subsubsection(node):
	global subsubsections
	subsubsections=subsubsections + 1
	name=chapstr(chapters)+'.'+str(sections)+'.'+str(subsections)+'.'+str(subsubsections)
	printWrapper('<h5 id="' + node.attrib["id"] + '"><span class="fill">'
		+ name + '</span>')
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
	printWrapper(node.text + '&#160;' + node.attrib["unit"] + node.tail)

def chapstr(chapter):
	chapnum=['A','B','C','D','E','F','G','H','I','J','K']
	if countmode:
		return chapnum[chapter-1]
	else:
		return str(chapter);

def listing(node):
	global listings
	global float_name
	global float_type
	global labels
	listings=listings + 1
	float_name=chapstr(chapters)+'.'+str(listings)
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
	printWrapper('''<pre><code''')
	if "language" in node.attrib:
		printWrapper(''' class="''' + node.attrib["language"] + '''"''')
	printWrapper('''>''')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('''</code></pre>''')
	if node.tail != None:
		printWrapper(node.tail)

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

def verbatiminput(node):
	global in_dir
	src=node.attrib['src']
	with open(in_dir + '/' + src, 'r') as content:
		data=content.read()
	printWrapper(html.escape(data,False))

def figure(node):
	global figures
	global float_name
	global float_type
	global labels
	figures=figures + 1
	float_name=chapstr(chapters)+'.'+str(figures)
	float_type='Figure'
	labels[node.attrib["id"]]=[float_type,float_name]
	printWrapper('''<figure id="''' + node.attrib["id"] + '''">
<picture>''')
	for altgraphics in node.findall('includegraphics'):
		includegraphics(altgraphics)
	last=node.findall('includegraphics[last()]')
	printWrapper('<img src="' + last[0].attrib["src"]+'" alt="'
		+node.findall('caption[last()]')[0].text+'">')
	printWrapper('</picture>')
	caption(node.findall('caption[last()]')[0])
	printWrapper('</figure>')

def herefigure(node):
	printWrapper('''<figure>
<picture>''')
	for altgraphics in node.findall('includegraphics'):
		includegraphics(altgraphics)
	last=node.findall('includegraphics[last()]')
	printWrapper('<img src="' + last[0].attrib["src"]+'" alt="'
		+last[0].attrib["src"]+'">')
	printWrapper('</picture>')
	printWrapper('</figure>')

def table(node):
	global tables
	global float_name
	global float_type

	tables=tables + 1
	float_name=chapstr(chapters)+'.'+str(tables)
	float_type='Table'
	labels[node.attrib["id"]]=[float_type,float_name]
	printWrapper('''<figure id="''' + node.attrib["id"] + '''">''')
	processElements(node)
	printWrapper('''</figure>''')

def tabular(node):
	printWrapper('''<table>\n''')
	processElements(node)
	printWrapper('''</table>\n''')

def colheaders(node):
	printWrapper('''<tr class="colheaders">''')
	processElements(node)
	printWrapper('''</tr>\n''')

def hline(node):
	printWrapper('''<tr class="hline">''')
	for k in range(0,int(node.attrib["colspan"])):
		printWrapper('''<td></td>''')
	printWrapper('''</tr>''')

def row(node):
	printWrapper('''<tr>''')
	processElements(node)
	printWrapper('''</tr>\n''')

def col(node):
	printWrapper('<td>')
	if node.text!=None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('</td>')

def defaultrule(node):
	printWrapper('''<''' + node.tag)
	for attr,value in node.attrib.items():
		printWrapper(''' ''' + attr + '''="''' + value + '''"''')
	printWrapper('''>''')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('''</''' + node.tag + '''>''')
	if node.tail != None:
		printWrapper(node.tail)

def infobox(node):
	printWrapper('''<div class="infobox_''' + node.attrib["type"] + ''' ">
<info-content>
<picture>
<source srcset="message_''' + node.attrib["type"] + '''.svg">
<img src="message_''' + node.attrib["type"] + '''.gif" alt="'''+node.attrib["type"]+'''">
</picture>
<div>''')
	if node.text != None:
		printWrapper(node.text)
	processElements(node)
	printWrapper('''</div>
</info-content></div>''')
	if node.tail != None:
		printWrapper(node.tail)

def appendix():
	global chapters
	global countmode
	countmode=1
	chapters=0

def processElements(document):
	for node in document.findall('*'):
		if node.tag=='title':
			title(node)
		if node.tag=='subtitle':
			subtitle(node)
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
		elif node.tag=='verbatiminput':
			verbatiminput(node)
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
		elif node.tag=='cmdoption':
			libname(node)
		elif node.tag=='function':
			libname(node)
		elif node.tag=='env':
			libname(node)
		elif node.tag=='class':
			libname(node)
		elif node.tag=='figure':
			figure(node)
		elif node.tag=='herefigure':
			herefigure(node)
		elif node.tag=='infobox':
			infobox(node)
		elif node.tag=='table':
			table(node)
		elif node.tag=='tabular':
			tabular(node)
		elif node.tag=='colheaders':
			colheaders(node)
		elif node.tag=='col':
			col(node)
		elif node.tag=='row':
			row(node)
		elif node.tag=='appendix':
			appendix()
		elif node.tag=='includegraphics':
			includegraphics2(node)
		elif node.tag=='chapter-star':
			chapter_star(node)
		elif node.tag=='hline':
			hline(node)
		else:
			defaultrule(node)

def main(argv):
	tree=ET.parse(sys.stdin)
	document=tree.getroot()
	args=json.loads(argv[0])
	global chapters
	global sections
	global subsection
	global subsubsections
	global listings
	global figures
	global tables
	global pass_counter
	global countmode
	global in_dir
	in_dir=args['in_dir']
	print('''<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>''' + document.findall('title')[0].text + '''</title>''')
	if 'stylesheets' in args:
		for style in args['stylesheets']:
			print('''<link rel="stylesheet" href="'''+style+'''" type="text/css">''')
	if 'scripts' in args:
		for script in args['scripts']:
			print('''<script src="'''+script+'''" type="text/javascript"></script>''')
	if 'scripts_inline' in args:
		for script in args['scripts_inline']:
			print(''''<script type="text/javascript"><!--'''+script+'''
//--></script>''')
	print('''</head>
<body>''')
	chapters=0
	sections=0
	subsections=0
	subsubsections=0
	listings=0
	figures=0
	tables=0

	processElements(document)

	countmode=0
	chapters=0
	sections=0
	subsections=0
	subsubsections=0
	listings=0
	figures=0
	tables=0

	pass_counter=pass_counter + 1

	processElements(document)

	print('''</body>
</html>''')



if __name__ == "__main__":
	main(sys.argv[1:])
