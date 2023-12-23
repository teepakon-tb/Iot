import paho.mqtt.client as mqtt
import json

host = "192.168.53.237"
port = 1883

# อ่านข้อมูลจากไฟล์ JSON
with open('db.json', 'r') as file:
    data = json.load(file)
    CInformation = data["CInformation"]

def update_money(id_to_update, deduction_amount):
    for item in CInformation:
        if item.get("id") == id_to_update:
            item["Money"] -= deduction_amount
            break

    # บันทึกข้อมูลที่ถูกอัปเดตกลับไปยังไฟล์ JSON
    with open('db.json', 'w') as file:
        json.dump(data, file, indent=4)

def on_connect(client, userdata, flags, rc):
    print("เชื่อมต่อกับ MQTT สำเร็จ")
    client.subscribe("id")

def on_message(client, userdata, msg):
    sub_value = int(msg.payload.decode("utf-8" , "strict"))

    # กำหนดจำนวนเงินที่จะหัก ซึ่งคาดว่าจะมาจาก frontend app.py
    deduction_amount = 100  # ให้แทนที่ด้วยจำนวนที่ต้องการหักจริง

    # ตรวจสอบว่า ID ที่ได้รับตรงกับ ID ในระบบหรือไม่
    valid_ids = [item["id"] for item in CInformation]
    if sub_value not in valid_ids:
        print("ไม่ใช่บัตรของทางร้าน")
        return

    # อัปเดตจำนวนเงินสำหรับ id ที่ระบุ
    update_money(sub_value, deduction_amount)

    # พิมพ์ข้อมูลที่ถูกอัปเดตเพื่อทำการตรวจสอบ
    for item in CInformation:
        if item.get("id") == sub_value:
            print(json.dumps(item, indent=4))
            break

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(host)
client.loop_forever()
