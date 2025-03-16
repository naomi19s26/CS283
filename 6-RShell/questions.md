How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines the end of an output by reading delimiters like "\n". To handle partial reads, the client should continue reading in a loop until the termination signal is received or the maximum length is reached.

This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol can mark message boundaries by adding the message length at the start, using special characters, or following a clear request-response pattern. If this is not handled properly, it could cause unexpected behavior, causing the client to wait for a long time for a long time

Describe the general differences between stateful and stateless protocols.

Stateful protocols remember information between requests, while stateless protocols treat each request independently.

Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is useful when low latency is important, such as when speed matters more than reliability

What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides socket APIs as an interface for network communication. This allows applications to use sockets to send and receive data over TCP or UDP. It enables programs to communicate over a network without handling low-level details.
