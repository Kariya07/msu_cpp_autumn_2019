import subprocess
def check(expr, result):
	out = subprocess.run(['./my_prog', expr], stdout = subprocess.PIPE)
	if out.stdout.decode('ascii') != result:
		print('error {} != {}, got {}'.format(expr, result, out.stdout))

check('2 + 432 * 10', '4322')
check('2 + 0*17 - 4/-2', '4')
check('4/2/2', '1')
check('17 + 27 / -9 / 3', '16')
check('21 - 5*3 + 12 / 0', 'Div by 0')
check('30 + 23 -', 'Expected number')
check('2 + 23*2 - +', 'Expected number')
check('2^3 + 8', 'Incorrect expression')

