import serial
import time
from colorama import init
from termcolor import colored

# use Colorama to make Termcolor work on Windows too
init()

ser = serial.Serial()
ser.port = 'COM4'
ser.baudrate = 115200
ser.open()

#very large string allocate and deallocate test

#test 1

test1 = ser.readline()
print(test1)
if test1 == b'2A6V7W5NL5ZZC6AYE84NKZ6MVFMZ5DZSYD9TM3\r\n':
	print(colored('success test 1.1','blue'))
else:
	print(colored('fail test 1.1','red'))
test1 = ser.readline()
print(test1)
if(test1 == b'00000800\r\n'):
	print(colored('success test 1.2','blue'))
else:
	print(colored('fail test 1.2','red'))
test1 = ser.readline()
print(test1)
if test1 == b'AENQ6TFME9R6JC47T94HXXK7758KKG5FN4A7PG47YKNMLUXR54UYJNTHZ9T3DAF6B9ELHBPS97NWZZTCVNDGMKV3WFAZ75Z6N7QE6WKHQ8DJR9HX56LLDMQX39NTA8TR89FS8794QSX9RNMEZ639NZVPPPCK2MXXGULX653GJVVT3SX3BV3NA687ZF9PE9ZVKN45373YSFTTS7G4HPV7CBECAYR24SEX4N7XTN8CUEXFYZB9WK3TC9Y3NR49DVWHHXJHZBSN5D2665GCNAJSDK4S4LTY42SNXZVAHSHECWRL7QFFC8ARET2HBDACUJJVS3X8ELKMZ98VEC9FN6VLBGP92NG7B8VH2LZQQ457NHP88RHVP5NNMY6CQWMS4HSX838NSLMJAVCYDPP2Y5F6MBFRUY4X5B894K7KZNQHRG5QHX35SE8A6JXCUF5999WNQJFXXDJR65CE3AWGUR3XD498E6WG3ZG6YTN6ATM7YBQ9LWWE7853PWA74QG6UMYQD7FFDA7NWGSCVNXX92ETRZEWKKJARV65Q7AM5SZ5E26U4D9CJU2YDJWXYX9WTPMMUM7L82FTME784EBYHGKAD7E8XT66SANF3394QEEK4JPJPFYFBDLD7RLF379THW6WBWV4CQRBVLXDAPDKM9ATY6D5U4QXC9FB7HRBZTKUSDKPJ5B4U2KED3EL4GAG57KB232G7HFR5CQ25KWV4LKZAQCDZGLNC2X8Y9FCVGZNPLQKZSKYP8GRYMQMH3266FHPMA4MSX27R5KY2JDBGGPFCHQFC2W65R\r\n':
	print(colored('success test 1.3','blue'))
else:
	print(colored('fail test 1.3','red'))

#test 2

test2_write = b'Hello my friend\r\n'
ser.write(test2_write)
test2_read = ser.readline()
print(test2_read)
if(test2_read == b'00000017\r\n'):
	print(colored('success test 2.1','blue'))
else:
	print(colored('fail test 2.1','red'))
test2_read = ser.readline()
print(test2_read)
if(test2_read == test2_write):
	print(colored('success test 2.2','blue'))
else:
	print(colored('fail test 2.2','red'))

test3_write = b'38GUJ8SQ3UQK343Q5WQYF6MQ2HP937TAYDE4WXDVFGRH6WHSZLJ86LQ3WJPYJJESR8T4GNFFHV4P8U293YAL38KGH8EWB26GALX79BDWSBYLZFEYG6VBZLBUS2NBKPQ9J8DRJ7ZN7UVJ488LXA9PLGA65BEY29NFCF3YPWRRDVFRNBH9A6Z8ZZEC93DN6XD9ZTSR2TMEX57VHTG3ZHJX5CF6959P9K9TLMQNETP3NVWPCR5KC93Z52QA5TXEATC3Y99UZHGHZCTFUZMKX7MCU9TLAJXPELC8SC6QRNG88BKM9WCPXETWH8D8PXZZUS6EEWAA4KFBZCGVDHRSGDYJJEJ9QQ3FAY5P2J8VMLWS6VU8W5XEVHE8VEUWY4THE3HX68ND9YZUVJTTLR2B7Y7DSX7E4WN6YB98SSS5K3PWZWSL82BKMA9Y8JFZNUAXBQN3V8ABEV2F8VHUVH6SJUESPQV4EVS3D2ARHENSYBUEX23Q2UN3Z\r\n'
ser.write(test3_write)
test3_read = ser.readline()
print(test3_read)
if(test3_read == b'00000499\r\n'):
	print(colored('success test 3.1','blue'))
else:
	print(colored('fail test 3.1','red'))
test3_read = ser.readline()
print(test3_read)
if(test3_read == test3_write):
	print(colored('success test 3.2','blue'))
else:
	print(colored('fail test 3.2','red'))

test4_write = b'ramses\r\n'
ser.write(test4_write)
test4_read = ser.readline()
print(test4_read)
if(test4_read == b'00000008\r\n'):
	print(colored('success test 4.1','blue'))
else:
	print(colored('fail test 4.1','red'))
test4_read = ser.readline()
print(test4_read)
if(test4_read == test4_write):
	print(colored('success test 4.2','blue'))
else:
	print(colored('fail test 4.2','red'))

ser.write(test3_write)
test5_read = ser.readline()
print(test5_read)
if(test5_read == b'00000499\r\n'):
	print(colored('success test 5.1','blue'))
else:
	print(colored('fail test 5.1','red'))
test5_read = ser.readline()
print(test5_read)
if(test5_read == test3_write):
	print(colored('success test 5.2','blue'))
else:
	print(colored('fail test 5.2','red'))

test6_write = b'38GUJ8SQ3UQK343Q5WQYF6MQ2HP937TAYDE4WXDVFGRH6WHSZLJ86LQ3WJPYJJESR8T4GNFFHV4P8U293YAL38KGH8EWB26GALX79BDWSBYLZFEYG6VBZLBUS2NBKPQ9J8DRJ7ZN7UVJ488LXA9PLGA65BEY29NFCF3YPWRRDVFRNBH9A6Z8ZZEC93DN6XD9ZTSR2TMEX57VHTG3ZHJX5CF6959P9K9TLMQNETP3NVWPCR5KC93Z52QA5TXEATC3Y99UZHGHZCTFUZMKX7MCU9TLAJXPELC8SC6QRNG88BKM9WCPXETWH8D8PXZZUS6EEWAA4KFBZCGVDHRSGDYJJEJ9QQ3FAY5P2J8VMLWS6VU8W5XEVHE8VEUWY4THE3HX68ND9YZUVJTTLR2B7Y7DSX7E4WN6YB98SSS5K3PWZWSL82BKMA9Y8JFZNUAXBQN3V8ABEV2F8VHUVH6SJUESPQV4EVS3D2ARHENSYBUEX23Q2UN3ZX\r\n'
ser.write(test6_write)
time.sleep(1)
print(ser.in_waiting)
if(ser.in_waiting == 0):
	print(colored('success test 6.0','blue'))
else:
	print(colored('fail test 6.0','red'))
