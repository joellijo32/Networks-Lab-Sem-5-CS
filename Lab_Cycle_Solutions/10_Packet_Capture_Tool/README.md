# Usage

1. Compile the program
	```bash
	gcc Q10_Capture.c -o capture
	```

2. Enter a new user & network namespace
	```bash
	unshare -Urn
	```

3. Bring up loopback (`down` by default)
	```bash
	ip link set lo up
	```

4. Run the program in background & generate traffic (in the same terminal)
	```bash
	./capture
	ping -c 1 127.0.0.1
	```

Without `sudo` previleges, no real network traffic can be analysed. Hence we're pinging the localhost of the device.
