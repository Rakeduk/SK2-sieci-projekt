import socket
from tkinter import * 
from time import sleep

IP = "127.0.0.1"
PORT = 4456
ADDR = (IP, PORT)
SIZE = 1024
FORMAT = "utf-8"

def main():    

    #########========GRAPHIC PART=============################
    root = Tk()

    def getSquareRoot ():
        """ TCP Socket """
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(ADDR)  
        data = myEntry.get()   

        """ Send data """
        client.send(data.encode(FORMAT))

        """ Recv data """
        data = client.recv(SIZE).decode(FORMAT)
        #print(f"[SERVER] {data}")

        lab1.config(text=data)
        
        """ Close connection """
        client.close()

    root.geometry("1000x300")
    lab1 = Label(root,text="Position 1", width=30)
    lab1.pack()

    lab2 = Label(root,text="Position 1", width=30)
    lab2.pack()

    lab3 = Label(root,text="Position 1", width=30)
    lab3.pack()

    lab4 = Label(root,text="Position 1", width=30)
    lab4.pack()

    lab5 = Label(root,text="Position 1", width=30)
    lab5.pack()

    lab6 = Label(root,text="Position 1", width=30)
    lab6.pack()

    lab7 = Label(root,text="Position 1", width=30)
    lab7.pack()

    lab8 = Label(root,text="Position 1", width=30)
    lab8.pack()


    myEntry = Entry(root,width = 30)
    myEntry.pack()

    myButton = Button(root, text="Click me!", command=getSquareRoot)
    myButton.pack()
        
    root.mainloop()

if __name__ == "__main__":
    main()
