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
vmsleep(2000);
sio.send("AT+CSOCKSETPN=1\r\n");		-- Set active PDP context’s profile number
result=sio.recv(2000);
print(result);
vmsleep(2000);
sio.send("AT+CIPMODE=0\r\n");		--TCPIP application mode (Set to Non-Transparent Mode (0))
result=sio.recv(2000);
print(result);
vmsleep(2000);
imei=getIMEI();
print(imei);
sio.send("AT+NETOPEN\r\n");		--opens packet network (Open socket)
result=sio.recv(2000);
print(result);
vmsleep(5000);
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
result=sio.recv(2000);
--git = result:sub(47,53);
print("Data Received: ");
--print(git);
--print("\r\n");
print(result);
print("Data Complete.\r\n");
--sio.send("AT+CIPOPEN?r\n");		--establish a connection with TCP server
--result=sio.recv(2000);
--print(string.len(result));

vmsleep(5000);

while(true)
do
  atctl.clear();
  receive=atctl.recv(500);				--The function is used to receive string from the port set by atctl.setport.

  sio.send("AT+CIPSEND=0,23\r\n");
  frame="$" .. imei .. "05" .. receive .. "00" .. "@";
  sio.send(frame);
  print(frame);
  print("\r\n");

  result=sio.recv(1000);
--  print(result);
  vmsleep(1000);

  if(result~=nil)
  then
    
    err1 = result:sub(3,15);
    res1="+IPCLOSE: 0,2";
    if (err1==res1)
    then
      print("Restarted..");
      vmsleep(5000);
      os.do_reset(true);
    end

    err2 = result:sub(3,40);
    res2="+CIPEVENT: NETWORK CLOSED UNEXPECTEDLY";
    if (err2==res2)
    then
      print("Restarted..");
      vmsleep(5000);
      os.do_reset(true);
    end
	
    data = result:sub(2,4);
 --  print(data);
    if (data=="IPD")
    then
      got = result:sub(8,11);
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
      if (got=="1100")
      then
        sending=atctl.send("d");
      end
    end
  end

  vmsleep(1000);
end

