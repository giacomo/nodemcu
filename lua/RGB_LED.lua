print("ColorPicker - WiFi PWM-Example")
wifi.setmode(wifi.STATION)
wifi.sta.config("YOUR_NETWORK_NAME","YOUR_NETWORK_PASSWORD")

function startup()
    if file.open("init.lua") == nil then
        print("init.lua deleted or renamed")
    else
        print("Running")
        file.close("init.lua")
        startServer()
    end
end

tmr.alarm(1, 1000, 1, function()
    if wifi.sta.getip() == nil then
        print("Waiting for IP address...")
    else
        tmr.stop(1)
        print("WiFi connection established, IP address: " .. wifi.sta.getip())
        print("You have 3 seconds to abort")
        print("Waiting...")
        tmr.alarm(0, 3000, 0, startup)
    end
end)

pwm.setup(1,1000,1023);
pwm.setup(2,1000,1023);
pwm.setup(3,1000,1023);
pwm.start(1);
pwm.start(2);
pwm.start(3);

function setColor(r, g, b)
    local red = 1023 - (r * 4 * 1023 / 1020);
    local green = 1023 - (g * 4 * 1023 / 1020);
    local blue = 1023 - (b * 4 * 1023 / 1020);
    pwm.setduty(1,red);
    pwm.setduty(3,green);
    pwm.setduty(2,blue);
end

function getColor()
    local redled = 255 - math.ceil(pwm.getduty(1) / 4 / 1023 * 1020);
    local greenled = 255 - math.ceil(pwm.getduty(3) / 4 / 1023 * 1020);
    local blueled = 255 - math.ceil(pwm.getduty(2) / 4 / 1023 * 1020);

    return string.format("%02x", redled)..string.format("%02x", greenled)..string.format("%02x", blueled)
end

function startServer()
    if srv then srv:close() end
    srv = net.createServer(net.TCP)
    srv:listen(80, function(conn)
      conn:on("receive", function(sck, req)
        -- local response = {}
        local _, _, method, path, vars = string.find(req, "([A-Z]+) (.+)?(.+) HTTP");
        if(method == nil)then
            _, _, method, path = string.find(req, "([A-Z]+) (.+) HTTP");
        end
        local _GET = {}
        if (vars ~= nil)then
            for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do
                _GET[k] = v
            end
        end

        local rgbValue = "FFFFFF";

        if(_GET.red ~= nil)then
            rgbValue = string.format("%02x", _GET.red)
            rgbValue = rgbValue..string.format("%02x", _GET.green)
            rgbValue = rgbValue..string.format("%02x", _GET.blue)

            setColor(_GET.red, _GET.green, _GET.blue)
        else
            rgbValue = getColor()
        end

        -- if you're sending back HTML over HTTP you'll want something like this instead
        local response = {"HTTP/1.0 200 OK\r\nServer: NodeMCU on ESP8266\r\nContent-Type: text/html\r\n\r\n"}

        response[#response + 1] = "<!doctype html><html class=\"no-js\" lang=\"\"><head>"
        response[#response + 1] = "<meta charset=\"utf-8\">"
        response[#response + 1] = "<title>ColorPicker - WiFi PWM-Example</title>"
        response[#response + 1] = "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.js\"></script>"
        response[#response + 1] = "<script>"
        response[#response + 1] = "function update(picker) {"
        response[#response + 1] = "var red = Math.round(picker.rgb[0]);"
        response[#response + 1] = "var green = Math.round(picker.rgb[1]);"
        response[#response + 1] = "var blue = Math.round(picker.rgb[2]);"
        response[#response + 1] = "console.log(red, green, blue);"
        response[#response + 1] = "window.location = '?red=' + red + '&green=' + green + '&blue=' + blue;"
        response[#response + 1] = "}"
        response[#response + 1] = "<\/script>"
        response[#response + 1] = "<\/head><body style=\"text-align: center;\">"
        response[#response + 1] = "<p><small>ColorPicker - WiFi PWM-Example<\/small><\/p>"
        response[#response + 1] = "<input class=\"jscolor {onFineChange:'update(this)'}\" value=\""..rgbValue.."\"\/>"
        response[#response + 1] = "<\/body><\/html>"

         -- sends and removes the first element from the 'response' table
        local function send()
          if #response > 0
            then sck:send(table.remove(response, 1))
          else
            sck:close()
          end
        end

        -- triggers the send() function again once the first chunk of data was sent
        sck:on("sent", send)
        send()
      end)
    end)
end
