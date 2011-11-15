def d(func):
	print('d()')
	return func
def dd(dd_val):
	def dd_inline(func):
		print('dd()' + str(dd_val))
		return func
	return dd_inline
def d_everytime(func):
	def d_everytime_inline():
		print('d_everytime_inline()')
		return func()
	return d_everytime_inline
def d_everytime_with_arg(val):
	def d_everytime_inline(func):
		def d_everytime_inline_inline():
			print('d_everytime_with_arg()')
			return func()
		return d_everytime_inline_inline
	return d_everytime_inline

@d
@dd(2)
def f():
	print('')
	pass

@d_everytime
@d_everytime_with_arg(3)
def f_everytime():
	pass

if '__main__' == __name__:
	f()
	f()
	f_everytime()
	f_everytime()
