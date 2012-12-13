import html.parser
class tHTMLParser(html.parser.HTMLParser):

    def handle_starttag(self, tag, attrs):
        print(tag, attrs)
    def handle_data(self, data):
        print(data)
    def handle_endtag(self, tag):
        print(tag)


if __name__ == '__main__':
    parser = tHTMLParser()
    parser.feed('<h1xxfff><f>hello</f></h1xxfff>')
