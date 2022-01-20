import socket
from tkinter import *
from time import sleep
import fcntl, os
import errno


IP = "127.0.0.1"
PORT = 4456
ADDR = (IP, PORT)
SIZE = 1024
FORMAT = "utf-8"


def main():
    #########========GRAPHIC PART=============################
    root = Tk()

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)
    fcntl.fcntl(client, fcntl.F_SETFL, os.O_NONBLOCK)

    def connectAndGetFirstCards():
        """ Send login """
        login = myEntry.get()
        client.send(login.encode(FORMAT))

        # """ Not blocking socket """
        while True:
            try:
                data = client.recv(SIZE).decode(FORMAT)
            except socket.error as e:
                err = e.args[0]
                if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
                    root.update()
                    continue
                else:
                    # a "real" error occurred
                    print(e)
                    sys.exit(1)
            else:
                print(f"[SERVER] {data}")
                res = data.split()
                if res[0] == "0":
                    lab5.config(text=str(res[1]))
                    lab6.config(text=str(res[2]))
                    lab7.config(text=str(res[3]))
                    root.update()
                else:
                    lab1.config(text=str(res[0]))
                    lab2.config(text=str(res[1]))
                    lab3.config(text=str(res[2]))
                    root.update()

        """ Close connection """
        # client.close()

    def doNothingYet():
        myCard = [lab5['text'], lab6['text'], lab7['text']]
        myAnswer = myEntry.get()
        if myAnswer in myCard:
            client.send(myAnswer.encode(FORMAT))
        else:
            print("Wrong input")

    root.geometry("1000x300")
    lab1 = Label(root, text="Position 1", width=30)
    lab1.pack()

    lab2 = Label(root, text="Position 1", width=30)
    lab2.pack()

    lab3 = Label(root, text="Position 1", width=30)
    lab3.pack()

    spacer_lab = Label(root, text="", width=30)
    spacer_lab.pack()

    lab4 = Label(root, text="A to twoje karta!", width=30)
    lab4.pack()

    lab5 = Label(root, text="Position 1", width=30)
    lab5.pack()

    lab6 = Label(root, text="Position 1", width=30)
    lab6.pack()
    lab7 = Label(root, text="Position 1", width=30)
    lab7.pack()

    myEntry = Entry(root, width=30)
    myEntry.pack()

    myButton = Button(root, text="Send answer!", command=doNothingYet)
    myButton.pack()

    myConnectButton = Button(root, text="Connect me!", command=connectAndGetFirstCards)
    myConnectButton.pack()

    root.mainloop()


if __name__ == "__main__":
    main()
