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

    def connectAndGetFirstCards ():
        """ TCP Socket """
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(ADDR)  
        data = client.recv(SIZE).decode(FORMAT)
        print(f"[SERVER] {data}")
        myCards = data.split()
        lab5.config(text=str(myCards[0]))
        lab6.config(text=str(myCards[1]))
        lab7.config(text=str(myCards[2]))

        login = myEntry.get()   

        """ Send login """
        client.send(login.encode(FORMAT))

        # """ Recv data """
        # data = client.recv(SIZE).decode(FORMAT)
        # print(f"[SERVER] {data}")
        # res = data.split()
        # lab1.config(text=str(res[0]))
        # lab2.config(text=str(res[1]))
        # lab3.config(text=str(res[2]))
        
        """ Close connection """
        #client.close()

    def doNothingYet():
        print("doNothingYet")

    root.geometry("1000x300")
    lab1 = Label(root,text="Position 1", width=30)
    lab1.pack()

    lab2 = Label(root,text="Position 1", width=30)
    lab2.pack()

    lab3 = Label(root,text="Position 1", width=30)
    lab3.pack()

    spacer_lab = Label(root,text="", width=30)
    spacer_lab.pack()

    lab4 = Label(root,text="A to twoje karta!", width=30)
    lab4.pack()

    lab5 = Label(root,text="Position 1", width=30)
    lab5.pack()

    lab6 = Label(root,text="Position 1", width=30)
    lab6.pack()
    lab7 = Label(root,text="Position 1", width=30)
    lab7.pack()

    


    myEntry = Entry(root,width = 30)
    myEntry.pack()

    myButton = Button(root, text="Click me!", command=doNothingYet)
    myButton.pack()

    myConnectButton = Button(root, text="Connect me!", command=connectAndGetFirstCards)
    myConnectButton.pack()
        
        
    root.mainloop()

if __name__ == "__main__":
    main()


