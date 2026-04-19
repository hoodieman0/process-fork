#ifndef MESSAGE_HPP
#define MESSAGE_HPP

/*
Request register message format:
signature: reg
type (byte): 1 for producer, 2 for consumer
id len : num
id: string

Confirm register message format:
signature: ack
id (byte): producer/consumer id

Failed register message format:
signature: nac


Control message format:
signature: ctrl
cmd (byte): num

Control cmds:
- 1: Start
- 2: Stop
- 3: Restart
- 4: Status
    - fd (int): fd to write status to
- 5: Dump
    - fd (int): fd to dump to

Message format from producer to broker:
signature: msg
producer id len: num
producer id: string
message len: num
message: string
null terminator: \0
*/



#endif // MESSAGE_HPP