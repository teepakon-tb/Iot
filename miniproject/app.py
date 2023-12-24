from flask import Flask, render_template,request, jsonify
import json
import paho.mqtt.publish as publish

app = Flask(__name__)

cart = []
with open('db.json', 'r') as json_file:
    data = json.load(json_file)
# MQTT broker configuration
mqtt_host = "192.168.53.237"
mqtt_port = 1883
mqtt_topic = "Total_price"

@app.route('/')
def shop():
    return render_template('index.html')
@app.route('/info')
def info():
    return render_template('info.html', data=data['CInformation'])
# Inside the /payment route
@app.route('/payment')
def payment():

    # Get total_price and cart_data from URL parameters
    total_price = float(request.args.get('total_price', 0))
    cart_data = request.args.get('cart_data', '[]')

    # Debugging: Print received data
    print("Received Total Price:", total_price)

    # Convert cart_data back to a list
    received_cart = json.loads(cart_data)

    # Convert total price to string before publishing
    total_price_str = str(total_price)

    # Publish total price to MQTT topic
    publish.single(mqtt_topic, total_price_str, hostname=mqtt_host, port=mqtt_port)

    return render_template('payment.html', total_price=total_price, total_quantity=len(received_cart), cart_data=json.dumps(received_cart))


if __name__ == '__main__':
    app.run(debug=True)