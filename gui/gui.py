import tkinter as tk
import subprocess
import atexit

def exit():
  process.stdin.write("exit\n".encode())
  process.stdin.flush()
  process.terminate()

def login_command():
  global process
  global is_open
  input_data = 'su ' + id_entry.get() + ' ' + pswd_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def logout_command():
  global process
  global is_open
  input_data = 'logout\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def useradd_command():
  global process
  global is_open
  input_data = 'register ' + id2_entry.get() + ' ' + pswd2_entry.get() + ' ' + username_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def register_command():
  global process
  global is_open
  input_data = 'register ' + id2_entry.get() + ' ' + pswd2_entry.get() + ' ' + userrank_entry.get() + ' ' + username_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def delete_command():
  global process
  global is_open
  input_data = 'delete ' + id2_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def passwordchange_command():
  global process
  global is_open
  input_data = 'passwd ' + id2_entry.get() + ' ' + pswd2_entry.get() + ' ' + passwordchange_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def showbook_command():
  global process
  global is_open
  num = 0
  tx = ''
  if bookname_entry.get() != '':
    num += 1
    tx = '-name="' + bookname_entry.get() + '"'
  if author_entry.get() != '':
    num += 1
    tx = '-author="' + author_entry.get() + '"'
  if ISBN_entry.get() != '':
    num += 1
    tx = '-ISBN=' + ISBN_entry.get() + ''
  if keyw_entry.get() != '':
    num += 1
    tx = '-keyword="' + keyw_entry.get() + '"'
  if num > 1:
    output_text.delete('1.0', tk.END)
    output_text.insert(tk.END, 'Please enter only one item')
    return
  input_data = 'show ' + tx + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output_text.delete('1.0', tk.END)
  output = "[ISBN]\t[BookName]\t[Author]\t[Keyword]\t[Price]\t[Quantity]\n"
  output_text.insert(tk.END, output)
  output = process.stdout.readline()
  while output != b'\r\n':
    output_text.insert(tk.END, output)
    output = process.stdout.readline()

def addbook_command():
  global process
  global is_open
  input_data = 'select ' + ISBN_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)
  tx = ''
  if bookname_entry.get() != '':
    tx += '-name="' + bookname_entry.get() + '" '
  if author_entry.get() != '':
    tx += '-author="' + author_entry.get() + '" '
  if keyw_entry.get() != '':
    tx += '-keyword="' + keyw_entry.get() + '" '
  input_data = 'modify ' + tx + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)


def buy_command():
  global process
  global is_open
  input_data = 'buy ' + ISBN_entry.get() + ' ' + quantity_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def select_command():
  global process
  global is_open
  input_data = 'select ' + ISBN_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def log_command():
  global process
  global is_open
  input_data = 'log' + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output_text.delete('1.0', tk.END)
  output = process.stdout.readline()
  while output != b'\r\n':
    output_text.insert(tk.END, output)
    output = process.stdout.readline()

def reportemployee_command():
  global process
  global is_open
  input_data = 'report employee' + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output_text.delete('1.0', tk.END)
  output = process.stdout.readline()
  while output != b'\r\n':
    output_text.insert(tk.END, output)
    output = process.stdout.readline()

def reportfinance_command():
  global process
  global is_open
  input_data = 'report finance' + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output_text.delete('1.0', tk.END)
  output = process.stdout.readline()
  while output != b'\r\n':
    output_text.insert(tk.END, output)
    output = process.stdout.readline()

def  showfinance_command():
  global process
  global is_open
  input_data = 'show finance' + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)
  output = process.stdout.readline()

def modify_command():
  global process
  global is_open
  tx = ''
  if bookname_entry.get() != '':
    tx += '-name="' + bookname_entry.get() + '" '
  if author_entry.get() != '':
    tx += '-author="' + author_entry.get() + '" '
  if ISBN_entry.get() != '':
    tx += '-ISBN=' + ISBN_entry.get() + ' '
  if keyw_entry.get() != '':
    tx += '-keyword="' + keyw_entry.get() + '" '
  input_data = 'modify ' + tx + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)

def import_command():
  global process
  global is_open
  input_data = 'import ' + quantity_entry.get()  + ' '  + totalcost_entry.get() + '\n'
  if not is_open:
    command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, cwd="D:\Studio\C\Bookstore-2023-main\gui")
    is_open = True
    atexit.register(exit)
  process.stdin.write(input_data.encode())
  process.stdin.flush()
  output = process.stdout.readline()
  output_text.delete('1.0', tk.END)
  output_text.insert(tk.END, output)


is_open = 0
command = "D:\Studio\C\Bookstore-2023-main\gui\code.exe"

root = tk.Tk()
root.title("Bookstore")

login_label = tk.Label(root, text="Login & out")
login_label.grid(row=0, column=1)

id_lable = tk.Label(root, text="ID")
id_lable.grid(row=1, column=0)

id_entry = tk.Entry(root)
id_entry.grid(row=1, column=1)

pswd_label = tk.Label(root, text="Password")
pswd_label.grid(row=2, column=0)

pswd_entry = tk.Entry(root)
pswd_entry.grid(row=2, column=1)

login_button = tk.Button(root, text="Login", command=login_command)
login_button.grid(row=3, column=1)

output_text = tk.Text(root, height=5, width=20)
output_text.grid(row=5, column=1, rowspan=20)

logout_button = tk.Button(root, text="Logout", command=logout_command)
logout_button.grid(row=4, column=1)

users_Label = tk.Label(root, text="Users")
users_Label.grid(row=0, column=3)

id2_lable = tk.Label(root, text="ID")
id2_lable.grid(row=1, column=2)

id2_entry = tk.Entry(root)
id2_entry.grid(row=1, column=3)

pswd2_label = tk.Label(root, text="Password")
pswd2_label.grid(row=2, column=2)

pswd2_entry = tk.Entry(root)
pswd2_entry.grid(row=2, column=3)

username_label = tk.Label(root, text="Username")
username_label.grid(row=3, column=2)

username_entry = tk.Entry(root)
username_entry.grid(row=3, column=3)

userrank_label = tk.Label(root, text="Userrank")
userrank_label.grid(row=4, column=2)

userrank_entry = tk.Entry(root)
userrank_entry.grid(row=4, column=3)

register_button = tk.Button(root, text="Register", command=register_command)
register_button.grid(row=5, column=3)

useradd_button = tk.Button(root, text="Useradd", command=useradd_command)
useradd_button.grid(row=6, column=3)

delete_button = tk.Button(root, text="Delete", command = delete_command)
delete_button.grid(row=7, column=3)

passwordchange_label = tk.Label(root, text="New Password")
passwordchange_label.grid(row=8, column=2)

passwordchange_entry = tk.Entry(root)
passwordchange_entry.grid(row=8, column=3)

passwordchange_button = tk.Button(root, text="Passwordchange", command=passwordchange_command)
passwordchange_button.grid(row=9, column=3)

book_Label = tk.Label(root, text="Book")
book_Label.grid(row=0, column=6)

bookname_label = tk.Label(root, text="Bookname")
bookname_label.grid(row=1, column=5)

bookname_entry = tk.Entry(root)
bookname_entry.grid(row=1, column=6)

author_label = tk.Label(root, text="Author")
author_label.grid(row=2, column=5)

author_entry = tk.Entry(root)
author_entry.grid(row=2, column=6)

ISBN_label = tk.Label(root, text="ISBN")
ISBN_label.grid(row=3, column=5)

ISBN_entry = tk.Entry(root)
ISBN_entry.grid(row=3, column=6)

keyw_label = tk.Label(root, text="Keyword")
keyw_label.grid(row=4, column=5)

keyw_entry = tk.Entry(root)
keyw_entry.grid(row=4, column=6)

price_label = tk.Label(root, text="Price")
price_label.grid(row=5, column=5)

price_entry = tk.Entry(root)
price_entry.grid(row=5, column=6)

quantity_label = tk.Label(root, text="Quantity")
quantity_label.grid(row=6, column=5)

quantity_entry = tk.Entry(root)
quantity_entry.grid(row=6, column=6)

addbook_button = tk.Button(root, text="Addbook", command=addbook_command)
addbook_button.grid(row=7, column=6)

showbook_button = tk.Button(root, text="Showbook", command=showbook_command)
showbook_button.grid(row=8, column=6)

selectbook_button = tk.Button(root, text="Selectbook", command=select_command)
selectbook_button.grid(row=9, column=6)

buybook_button = tk.Button(root, text="Buybook", command=buy_command)
buybook_button.grid(row=10, column=6)

modifybook_button = tk.Button(root, text="Modifybook", command=modify_command)
modifybook_button.grid(row=11, column=6)

totalcost_label = tk.Label(root, text="Totalcost")
totalcost_label.grid(row=12, column=5)

totalcost_entry = tk.Entry(root)
totalcost_entry.grid(row=12, column=6)

importbook_button = tk.Button(root, text="Importbook", command=import_command)
importbook_button.grid(row=13, column=6)

log_label = tk.Label(root, text= "log")
log_label.grid(row = 0, column=8)

count_label = tk.Label(root, text="Count")
count_label.grid(row= 1, column=7)

count_entry = tk.Entry(root)
count_entry.grid(row = 1, column=8)

log_button = tk.Button(root, text="Log", command=log_command)
log_button.grid(row=2, column=8)

showfinance_button = tk.Button(root, text="Showfinance", command=showfinance_command)
showfinance_button.grid(row=3, column=8)

reportfinance_button = tk.Button(root, text="Reportfinance", command=reportfinance_command)
reportfinance_button.grid(row=4, column=8)

reportemployee_button = tk.Button(root, text="Reportemployee", command=reportemployee_command)
reportemployee_button.grid(row=5, column=8)

root.mainloop()