import serial

# 输出支持
from rich.console import Console
from rich.table import Table
from rich.panel import Panel
from rich.text import Text

# 显示可用串口
def show_serial() -> str:
    sl = []
    for i in range(256):
        try:
            ser = serial.Serial('COM'+str(i))
            sl.append('COM'+str(i))
            ser.close()
        except serial.SerialException:
            pass
    return sl

# 读取串口数据
def read_serial(ser: serial.Serial) -> str:
    data = b''
    data += ser.readline()
    if data[-2:] == b'\r\n':
        print('接收RAW: ',data)
        return str(data[:-2], encoding='utf-8')

def SendFrame(ser:serial.Serial, frame:bytes):
    ser.write(frame)
    

# 成帧
def MakeFrame(data:str,S_n:int) -> bytes:
    data += chr(len(data))
    data += '\r\n'
    data = str(S_n) + data
    print('发送RAW：',bytes(data, encoding='utf-8'))
    return bytes(data, encoding='utf-8')


def corrupted(frame:str) -> bool:
    # 先简单写一下
    if frame is None:
        return True
    return  len(frame) != 6 or frame[:3] not in ['ACK','NAK'] or frame[5] not in ['0','1','2','3','4','5','6','7','8']


def isInWindow(i, S_n, S_f, S_w) -> bool:  # 判断是否在窗口内, Sf < i <= Sn
    if 0 <= S_n - S_f <= S_w//2:
        return S_f < i <= S_n
    else:
        return i <= S_n or S_f < i <= S_n+S_w



# 选择性重传ARQ发送方算法
def ARQ_send(ser:serial.Serial):
    console = Console()
    console.print(Panel(' === Selective Repeat ARQ experiment ===\nby framsit'),justify='center')
    S_w = 2**4//2       # 8 注意这是窗口大小的两倍
    S_f = 0
    S_n = 0
    frames = ['']*S_w
    framesTime = [0]*S_w

    
    while True: # Repeat forever
        # frame = input('请输入发送(q退出)：')
        frame = console.input(Text('\n请输入发送(q退出)：',style='green bold'))
        if frame == '': 
            console.print('[!] 输入为空',style='red bold')
            continue
        if frame == 'q':  
            console.print(Panel(' === experiment end ==='),justify='center')
            break


        # 有包要发送
        if(abs(S_n - S_f) == S_w//2): # 窗口满
            console.print(Text('[!] 窗口满，等待空，丢弃输入',style='red bold'))
        else:
            # 发送
            frames[S_n] = frame
            SendFrame(ser,MakeFrame(frame,S_n))
            framesTime[S_n] = 0
            S_n += 1
            S_n %= S_w
        

        # 接收
        while True:
            read_frame = read_serial(ser)
            if corrupted(read_frame):
                console.print('[!] 接收到的数据有误或丢失',style='red bold')
            else:
                print('接收到数据正确:',read_frame)
                ackNo = int(read_frame[5],16)
                if read_frame[:3] == 'NAK' and isInWindow(ackNo+1,S_n,S_f,S_w):
                    console.print('接收到NAK，重传',style='yellow')
                    SendFrame(ser,MakeFrame(frames[ackNo],ackNo))
                    framesTime[ackNo] = 0
                    continue

                if read_frame[:3] == 'ACK': 
                    console.print(f'接收到ACK:{ackNo}',style='blue')
                    if isInWindow(ackNo,S_n,S_f,S_w):
                        while(ackNo != S_f):
                            frames[S_f] = '' # purge
                            framesTime[S_f] = 0
                            S_f += 1
                            S_f %= S_w

            # 超时 这里的时间抽象为经过循环的次数（方便测试）
            for i in range(S_w):
                if frames[i] != '':
                    if framesTime[i] > 4:
                        console.print(f'超时重传 No.{i}: {frames[i]}',style='yellow')
                        SendFrame(ser,MakeFrame(frames[i],i))
                        framesTime[i] = 0
                        break
                    else:
                        framesTime[i] += 1
            else:
                break # 都没要再发再接受的，退出接收循环

        # 打印状态
        print('S_n:',S_n,'S_f:',S_f)
        table = Table(show_header=True, header_style='blue', title=f'窗口大小:4  S_n:{S_n}  S_f:{S_f}')
        table.add_column('No')
        table.add_column('Frame')
        table.add_column('Time')
        for i in range(S_w):
            s = 'red' if isInWindow(i+1,S_n,S_f,S_w) else None
            table.add_row(str(i),str(frames[i]),str(framesTime[i]),style=s)
        console.print(table,justify='center')

        


def main():
    sl = show_serial()
    port = 'COM' + input('找到串口：'+str(sl)+' 请输入串口号：COM') if len(sl) != 1 else sl[0]
    baudrate = 9600
    ser = serial.Serial(port, baudrate, timeout=0.5)
    print('串口已打开:',ser.name,ser.baudrate,ser.timeout)
    
    ARQ_send(ser)
    
    ser.close()

if __name__ == '__main__':
    main()

