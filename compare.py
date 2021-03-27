epochsize = 50000000
global_pointer = 0
windowsize = 9
progressgap = 100000

if __name__ == '__main__':
    if epochsize < windowsize:
        raise BaseException

    pifile=open('Pi - Dec - Chudnovsky.txt', 'r', encoding='UTF-8')
    uids=[]
    with open('reposts.txt', 'r') as repost_file:
        uids = repost_file.read().splitlines()
    print('已载入的uid数目为',len(uids))
    print('窗口宽度为', windowsize)
    print('一次读取的字节数为', epochsize)

    Lastbuffer=''
    pointer=0
    count=1

    while 1:
        readresult = pifile.read(epochsize)
        if readresult == '':
            print('读取结束!')
        else:
            if Lastbuffer != '':
                pointer=0
                Lastbuffer += readresult
                while pointer < windowsize:
                    if global_pointer % progressgap == 0:
                        print('当前窗口是',Lastbuffer[pointer:pointer+windowsize],', 指针在小数点后',global_pointer-1,'位', end='\r')
                    if Lastbuffer[pointer:pointer+windowsize] in uids:
                        print('第%d个命中的uid是%d' % (count, int(readresult[pointer:pointer+windowsize])))
                        count+=1
                    pointer+= 1
                    global_pointer+=1
            pointer=0
            Lastbuffer=''
            while pointer < epochsize-windowsize:
                if global_pointer % progressgap == 0:
                    print('当前窗口是',readresult[pointer:pointer+windowsize],', 指针在小数点后',global_pointer-1,'位', end='\r')
                if readresult[pointer:pointer+windowsize] in uids:
                    print('第%d个命中的uid是%d' % (count, int(readresult[pointer:pointer+windowsize])), ', 指针在小数点后',global_pointer-1,'位')
                    count+=1
                pointer += 1
                global_pointer+=1
            Lastbuffer=readresult[epochsize-windowsize:epochsize]