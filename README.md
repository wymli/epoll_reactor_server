# epoll_reactor_server
基于epoll的reactor模式的TCP_Server

- 目前,单进程执行callback_handler
- 三个callback处理线程
  ```c
  void callback_recvdata(int epfd, my_event_st *ev);
  void callback_accept(int epfd, my_event_st *ev);
  void callback_senddata(int epfd, my_event_st *ev);
  ```


- Server主函数:
  ```c
  my_event_st my_events[MAX_EVENTS + 1];
  int my_events_len = 0;  

  int main()
  {
    int epfd = epoll_create(MAX_EVENTS + 1);
    int ls_fd = create_ls_socket(SERV_PORT);
    my_event_st *ls_ev = create_my_event(ls_fd, EPOLLIN, (void *)callback_accept);
    register_event(epfd, ls_ev);
    epoll_event events[MAX_EVENTS + 1];
    while (1)
    {
      int nfd = epoll_wait(epfd, events, MAX_EVENTS + 1, ONE_MINUTE);
      perr(nfd <= 0, "epoll_wait");
      for (int i = 0; i < nfd; i++)
      {
        my_event_st *ev = events[i].data.ptr;
        ev->callback_fn(epfd, ev);
      }
    }
  }
  ```
  ---
  需要注意的坑:
  - 如果client关闭了socket,那么server会收到长度为0的msg,即会调用callback_recvdata,这个时候需要在函数里面对长度为0进行判断,并取消注册,关闭fd