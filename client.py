import tkinter as tk
from tkinter import messagebox
import socket
import threading

# network client
client = None
HOST_ADDR = "192.168.1.3"
HOST_PORT = 4456

window = tk.Tk()
window.title("Client")
username = " "


topFrame = tk.Frame(window)
lblName = tk.Label(topFrame, text = "Name:").pack(side=tk.LEFT)
entName = tk.Entry(topFrame)
entName.pack(side=tk.LEFT)
btnConnect = tk.Button(topFrame, text="Connect", command=lambda : connect())
btnConnect.pack(side=tk.LEFT)
#btnConnect.bind('<Button-1>', connect)
topFrame.pack(side=tk.TOP)


middleFrame = tk.Frame(window)  

allCardsFrame = tk.Frame(middleFrame)

mainCardFrame = tk.Frame(allCardsFrame)
lblLine = tk.Label(mainCardFrame, text="*********************************************************************").pack()
lab1 = tk.Label(mainCardFrame, text="first element", width=20, height = 10)
lab1.pack(side=tk.LEFT)
lab2 = tk.Label(mainCardFrame, text="second element", width=20, height = 10)
lab2.pack(side=tk.LEFT)
lab3 = tk.Label(mainCardFrame, text="third element", width=20, height = 10)
lab3.pack(side=tk.LEFT)
mainCardFrame.pack(side=tk.TOP)

myCardsFrame = tk.Frame(allCardsFrame)
lblLine = tk.Label(myCardsFrame, text="***Your cards!***").pack()
lab5 = tk.Label(myCardsFrame, text="first card", width=20, height = 10)
lab5.pack(side=tk.LEFT)
lab6 = tk.Label(myCardsFrame, text="second second", width=20, height = 10)
lab6.pack(side=tk.LEFT)
lab7 = tk.Label(myCardsFrame, text="third card", width=20, height =10)
lab7.pack(side=tk.LEFT)
myCardsFrame.pack(side=tk.TOP)

allCardsFrame.pack(side=tk.LEFT)

    
rankingFrame = tk.Frame(middleFrame)
tkDisplay = tk.Text(rankingFrame, height=20, width = 10)
tkDisplay.pack(side=tk.LEFT, fill=tk.X)
tkDisplay.tag_config("tag_your_message", foreground="blue")
rankingFrame.pack(side=tk.LEFT)

middleFrame.pack(side=tk.TOP)


bottomFrame = tk.Frame(window)
tkMessage = tk.Text(bottomFrame, height=2, width=40)
tkMessage.pack(side=tk.LEFT, padx=(5, 13), pady=(5, 10))
tkMessage.config(highlightbackground="grey", state="disabled")
tkMessage.bind("<Return>", (lambda event: getChatMessage(tkMessage.get("1.0", tk.END))))
bottomFrame.pack(side=tk.BOTTOM)


def connect():
    global username, client
    if len(entName.get()) < 1:
        tk.messagebox.showerror(title="ERROR!!!", message="You MUST enter your first name <e.g. John>")
    else:
        username = entName.get()
        connect_to_server(username)




def connect_to_server(name):
    global client, HOST_PORT, HOST_ADDR
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((HOST_ADDR, HOST_PORT))
        client.send(name.encode()) # Send name to server after connecting

        entName.config(state=tk.DISABLED)
        btnConnect.config(state=tk.DISABLED)
        tkMessage.config(state=tk.NORMAL)

        # start a thread to keep receiving message from server
        # do not block the main thread :)
        threading._start_new_thread(receive_message_from_server, (client, "m"))
    except Exception as e:
        tk.messagebox.showerror(title="ERROR!!!", message="Cannot connect to host: " + HOST_ADDR + " on port: " + str(HOST_PORT) + " Server may be Unavailable. Try again later")


def receive_message_from_server(sck, m):
    while True:
        from_server = sck.recv(64).decode()

        if not from_server: break

        print(f"[SERVER] {from_server}")
        res = from_server.split()
        if res[0] == "0":
            lab5.config(text=str(res[1]))
            lab6.config(text=str(res[2]))
            lab7.config(text=str(res[3]))
            window.update()
        elif res[0] == "1":
            del res[0]
            print(res)
            tkDisplay.config(state=tk.NORMAL)
            texts = tkDisplay.delete("1.0", tk.END)

            for i in range(len(res)):
                if i%2 == 0:
                    print("nieparzysta" + res[i])
                    tkDisplay.insert(tk.END, res[i] + " ")
                else:
                    tkDisplay.insert(tk.END, res[i] + "\n")

            tkDisplay.config(state=tk.DISABLED)
            tkDisplay.see(tk.END)
        else:
            lab1.config(text=str(res[0]))
            lab2.config(text=str(res[1]))
            lab3.config(text=str(res[2]))
            window.update()
        

    sck.close()
    window.destroy()


def getChatMessage(msg):

    msg = msg.replace('\n', '')
    texts = tkDisplay.get("1.0", tk.END).strip()

    # enable the display area and insert the text and then disable
    tkDisplay.config(state=tk.NORMAL)
    if len(texts) < 1:
        tkDisplay.insert(tk.END, "You->" + msg, "tag_your_message") # no line
    else:
        tkDisplay.insert(tk.END, "\n\n" + "You->" + msg, "tag_your_message")

    tkDisplay.config(state=tk.DISABLED)

    send_mssage_to_server(msg)

    tkDisplay.see(tk.END)
    tkMessage.delete('1.0', tk.END)


def send_mssage_to_server(msg):
    client_msg = str(msg)
    client.send(client_msg.encode())
    if msg == "exit":
        client.close()
        window.destroy()
    print("Sending message")


window.mainloop()
