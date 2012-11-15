import markdown
import sys
md = markdown.Markdown()
f = open(sys.argv[1])
print md.convert(f.read())
f.close()
