import pyautogui as pag
import serial
import win32com.client as comclt

arduino = serial.Serial("COM5", 9600) # Escuchamos en el puerto COM en el que esta el arduino a 9600 baudios.

# Este bloque de codigo recibe una cadena de caracteres que es enviada por el arduino y
# la procesa hasta dejar en limpio solo el valor que nos interesa.
while (1):
    while (arduino.inWaiting()==0):
        pass
    dato = arduino.readline()
    a = dato.splitlines()
    b = str(a[0])
    c = b.replace("b","")
    d = c.replace("'","")
    e = int(d) # En esta variable se guarda el valor final enviado por el arduino.
    print(e)

#----------------------------------------Eje Z del MPU6050.
    # Si se recibe un "80" desde el arduino, se ejecuta el siguiente bloque de código
    # que simula la pulsacion de una tecla.
    if e == 80:
        wsh= comclt.Dispatch("WScript.Shell")
        wsh.SendKeys("a") # Tecla que se simula.

    elif e == 70:
        wsh= comclt.Dispatch("WScript.Shell")
        wsh.SendKeys("b")

#----------------------------------------Eje Y del MPU6050.
    if e == 50: #Arriba.
        pag.move(0, 10) # Cantidad de pixeles que se mueve el cursor del mouse.

    elif e == 40: #Abajo.
        pag.move(0, -10)
    
#----------------------------------------Eje X del MPU6050.
    if e == 10: #Derecha.
        pag.move(10, 0)

    elif e == 20: #Izquierda.
        pag.move(-10, 0)

    e = 0