run2:
	@clear
	@gcc admin.c cal-new.c -w  -pthread -o admin.exe
	@gcc client.c -o client.exe