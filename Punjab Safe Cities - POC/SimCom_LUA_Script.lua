printdir(1)
vmsleep(10000);

function getIMEI()
	sio.send("AT+SIMEI?\r\n");
	imeidata = sio.recv(1000);
	print(imeidata);
    imei1 = imeidata:sub(21,35);
    return imei1;
end

repeat
uartport = atctl.setport(1);
print(uartport);
until(uartport==true)

sio.send("(AT+CGSOCKCONT=1,\"IP\",\"wap.mobilinkworld.com\"\r\n)");		-- Define socket PDP context
result=sio.recv(2000);
print(result);
vmsleep(1000);
sio.send("AT+CSOCKSETPN=1\r\n");		-- Set active PDP context’s profile number
result=sio.recv(2000);
print(result);
vmsleep(1000);
sio.send("AT+CIPMODE=0\r\n");		--TCPIP application mode (Set to Non-Transparent Mode (0))
result=sio.recv(2000);
print(result);
vmsleep(1000);
imei=getIMEI();
print(imei);
sio.send("AT+NETOPEN\r\n");		--opens packet network (Open socket)
result=sio.recv(2000);
print(result);
vmsleep(2000);
sio.send("AT+IPADDR\r\n");		--inquires the IP address of current active socket PDP.
result=sio.recv(2000);
print(result);
vmsleep(2000);
sio.send("AT+CIPSRIP=0\r\n");
result=sio.recv(2000);
print(result);
vmsleep(2000);
--sio.send("AT+CIPOPEN=0,\"TCP\",\"203.215.160.163\",5056\r\n");		--establish a connection with TCP server
sio.send("AT+CIPOPEN=0,\"TCP\",\"203.215.160.163\",2570\r\n");		--establish a connection with TCP server
result=sio.recv(2500);

print("Data Received: ");
result=sio.recv(2500);
--print(result);
print("Data Complete.\r\n");


git = result:sub(3,15);

if(git ~= "+CIPOPEN: 0,0")
then
  print("Not Connected to Server,Restarting the device...\r\n");
  vmsleep(2500);
  os.do_reset(true);
end

vmsleep(1000);

while(true)
do
  result=sio.recv(500);
--  print(result);
--  vmsleep(1000);

  if(result~=nil)
  then
    err1 = result:sub(3,15);
    err1a = result:sub(1,13);
    err2 = result:sub(3,40);
    err3 = result:sub(3,25);
    data = result:sub(2,4);

    if (err1=="+IPCLOSE: 0,1")
    then
      print("Restarting..\r\n");
      vmsleep(2500);
      os.do_reset(true);
    end

    if (err1=="+IPCLOSE: 0,2")
    then
      print("Restarting..\r\n");
      vmsleep(2500);
      os.do_reset(true);
    end

    if (err1a=="+IPCLOSE: 0,2")
    then
      print("Restarting..\r\n");
      vmsleep(2500);
      os.do_reset(true);
    end

    if (err2=="+CIPEVENT: NETWORK CLOSED UNEXPECTEDLY")
    then
      print("Restarting..\r\n");
      vmsleep(2500);
      os.do_reset(true);
    end

    if (err3=="+SIMCARD: NOT AVAILABLE")
    then
      print("Restarting..\r\n");
      vmsleep(2500);
      os.do_reset(true);
    end

    if (data=="IPD")
    then
      got = result:sub(8,11);
--  print(data .. "\r\n");
--  print(got.. "\r\n");
      if (got=="0000")
      then
        sending=atctl.send("a");
      end
      if (got=="0100")
      then
        sending=atctl.send("b");
      end
      if (got=="1000")
      then
        sending=atctl.send("c");
      end
      if (got=="1100" or got=="1111")
      then
        sending=atctl.send("d");
	  end
      if (got=="1024")
      then
		print("WRONG.....!!!!!");
	  end
    print("Data Sent.\r\n");
    end
--  else
--    print("No Data Received.\r\n");
  end

  atctl.clear();
  receive=atctl.recv(1000);				--The function is used to receive string from the port set by atctl.setport.
--  print("Data received through UART: " .. receive .. "\r\n");

  len = string.len(receive);
--  print("String Length: " .. len .. "\r\n");

--  if(len ~= 37)
--  then
--    print("Incorrect frame received.\r\n");
--  else
--    print("Frame Created: " .. frame .. "\r\n");
--  end

  Lat = 33.65891861;
  Lon = 73.05744605;

  sio.send("AT+CIPSEND=0,78\r\n");
  frame="$" .. imei .. "01" .. Lat .. Lon .. receive .. "@";
  sio.send(frame);
  print("Data Sent: " .. frame .. "\r\n");

--  vmsleep(3000);
end

