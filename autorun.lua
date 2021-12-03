printdir(1)
vmsleep(5000);

sio.send("(AT+CGPS=1,1\r\n)");		-- Define socket PDP context
result=sio.recv(1000);
print(result);
	
while(true)
do
	sio.send("AT+CGPSINFO\r\n");		-- Set active PDP context’s profile number
	result=sio.recv(1000);
	print(result);

    vmsleep(3000);
end

