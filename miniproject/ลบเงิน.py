import paho.mqtt.client as mqtt
import json

host = "192.168.53.237"
port = 1883

# Read data from the JSON file
with open('db.json', 'r', encoding='utf-8') as file:
    data = json.load(file)
    CInformation = data["CInformation"]

# Set a default value for id_to_update
id_to_update = None

def update_money(id_to_update, deduction_amount):
    for item in CInformation:
        if item.get("id") == id_to_update:
            item["Money"] -= deduction_amount
            break  # Stop the loop once the correct item is found and updated

    # Save the updated data back to the JSON file
    with open('db.json', 'w', encoding='utf-8') as file:
        json.dump(data, file, ensure_ascii=False, indent=4)

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT successfully")
    client.subscribe("Total_price")
    client.subscribe("id")

def on_message(client, userdata, msg):
    global id_to_update

    if msg.topic == "id":
        received_id = int(msg.payload.decode("utf-8", "strict"))

        if received_id not in [1, 2]:
            print(f"Invalid ID: {received_id}. Not a valid store card.")
            return

        if id_to_update is not None:
            print("Ignoring 'id' topic data, waiting for previous update to complete.")
        else:
            id_to_update = received_id
            print(f"Received valid ID: {id_to_update}. Waiting for deduction amount from 'Total_price' topic.")

    elif msg.topic == "Total_price":
        if id_to_update is None:
            print("Waiting for valid 'id' data before processing deduction.")
        else:
            deduction_amount = float(msg.payload.decode("utf-8", "strict"))
            update_money(id_to_update, deduction_amount)

            # Print the updated information
            print("Updated information:")
            for item in CInformation:
                print(json.dumps(item, indent=4))

            # Print the total deducted amount
            total_deducted_amount = deduction_amount
            print(f"Total deducted amount: {total_deducted_amount} Bath")
            id_to_update = None  # Reset id_to_update after update is complete

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(host, port, keepalive=60)
client.loop_forever()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(host, port, keepalive=60)
client.loop_forever()
