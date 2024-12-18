<template>
  <div id="forum-page">
      <!-- 侧边栏 -->
      <aside class="sidebar">
        <div class="profile">
          <img src="../assets/高木.png" alt="用户头像" class="avatar" />
          <h3>祝你所求有所得</h3>
        </div>
        <nav>
          <router-link to="/profile" class="nav-item">个人中心</router-link>
          <router-link to="/favorites" class="nav-item">收藏夹</router-link>
          <router-link to="/listings" class="nav-item">我的发布</router-link>
          <router-link to="/forum" class="nav-item">论坛</router-link>
        </nav>
      </aside>  
    <main class="forum-content">
      <header class="forum-header">
        <h2>群聊论坛</h2>
      </header>
  
      <!-- 消息框 -->
      <div class="chat-box">
        <div class="messages" ref="messageContainer">
          <div v-for="message in messages" :key="message.timestamp" :class="['message', { 'my-message': message.user === currentUser }]">
            <p><strong>{{ message.user }}:</strong> {{ message.text }}</p>
            <span class="timestamp">{{ formatTimestamp(message.timestamp) }}</span>
          </div>
        </div>
  
        <!-- 输入框 -->
        <div class="input-box">
          <input v-model="newMessage" @keyup.enter="sendMessage" placeholder="Type your message here..." />
          <button @click="sendMessage">Send</button>
        </div>
      </div>
    </main>
  </div>
</template>

<script>
export default {
  data() {
    return {
      messages: [], // 消息列表
      newMessage: '', // 当前输入的消息
      socket: null, // WebSocket 连接对象
      currentUser: sessionStorage.getItem('username') || '', // 当前用户的用户名，假设已存储在 sessionStorage 中
    };
  },
  mounted() {
    console.log('Current user:', this.currentUser);

    this.fetchMessages();
    
    const uid = this.currentUser ? this.currentUser : 0;  // 假设 currentUser 中有 id 字段
    this.socket = new WebSocket(`ws://localhost:3000/chat?uid=${uid}`);

    // WebSocket 连接成功时触发
    this.socket.onopen = () => {
      console.log('WebSocket connection established');
    };

    // 接收到服务器消息时处理
    this.socket.onmessage = (event) => {
      console.log('Received message:', event.data);
      const message = JSON.parse(event.data); // 假设后端发送的是 JSON 格式的消息
      this.messages.push(message);  // 将消息添加到本地消息列表
      this.scrollToBottom(); // 新消息到来时自动滚动到底部
    };

    // WebSocket 连接关闭时触发
    this.socket.onclose = () => {
      console.log('WebSocket connection closed');
    };

    // 错误处理
    this.socket.onerror = (error) => {
      console.error('WebSocket error:', error);
    };
  },
  methods: {
    // 获取聊天记录
    fetchMessages() {
      fetch('/get-message')  // 调用后端接口
        .then(response => response.json())  // 将响应解析为 JSON
        .then(data => {
          if (data.info) {
            this.messages = data.info;  // 将后端返回的消息设置到前端
          } else {
            console.log('No previous messages');
          }
        })
        .catch(error => {
          console.error('Error fetching messages:', error);
        });
    },    
    sendMessage() {
      const username = sessionStorage.getItem('username'); // 从 sessionStorage 获取当前用户名
      if (this.newMessage.trim() !== '') {
        const message = {
          user: username,
          text: this.newMessage,
          timestamp: new Date().toISOString(),
        };
        // 发送消息到服务器
        this.socket.send(JSON.stringify(message));
        // 将消息添加到本地消息列表
        this.messages.push(message);
        console.log('Sent message:', message);
        this.newMessage = ''; // 清空输入框

        // 滚动到消息区域底部
        this.scrollToBottom();
      }
    },
    scrollToBottom() {
      const container = this.$refs.messageContainer;
      if (container) {
        container.scrollTop = container.scrollHeight;
      }
    },

    // 格式化时间戳的方法
    formatTimestamp(timestamp) {
      const date = new Date(timestamp);
      return date.toLocaleString();
    }
  },
};
</script>

<style scoped>
/* 清除所有元素的默认 margin 和 padding */
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box; /* 确保元素大小包含边框和内边距 */
}
/* 确保 html 和 body 的高度为100% */
html, body {
  margin: 0 !important;
  padding: 0;
  height: 100%;
  background-color: #34495e;
}
/* 主页容器，使用 flexbox 布局 */
#forum-page {
  display: flex;
  height: 100vh; /* 让页面占满视口 */
  font-family: 'Roboto', sans-serif;
  background-color: #34495e;
  color: #ecf0f1;
}

/* 侧边栏 */
.sidebar {
  width: 260px; /* 固定宽度 */
  height: 100vh; /* 让侧边栏高度填满页面 */
  background-color: #2c3e50;
  padding: 1.5rem;
  box-shadow: 2px 0 8px rgba(0, 0, 0, 0.2);
  border-radius: 8px;
  z-index: 10; /* 保证侧边栏位于内容区上方 */
  position: fixed; /* 固定位置 */
  top: 0;
  left: 0;
  overflow: hidden; /* 侧边栏不显示滚动条 */
}

/* 用户头像和个人信息 */
.profile {
  text-align: center;
  margin-bottom: 2rem;
}

.avatar {
  width: 80px;
  height: 80px;
  border-radius: 50%;
  margin-bottom: 0.5rem;
}

/* 导航项 */
.nav-item {
  display: block;
  padding: 0.8rem 1rem;
  margin-bottom: 0.8rem;
  color: #ecf0f1;
  text-decoration: none;
  border-radius: 12px;
  transition: background-color 0.3s ease;
}

.nav-item:hover {
  background-color: #16a085;
  color: white;
}

/* 论坛内容区，flexbox布局 */
.forum-content {
  margin-left: 260px; /* 确保内容区从侧边栏的右边开始 */
  padding: 0; /* 去除内边距 */
  height: 100vh; /* 内容区填满页面 */
  overflow-y: auto; /* 允许内容区滚动 */
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
  background-color: #34495e; /* 确保背景色和页面一致 */
  flex-grow: 1; /* 内容区占据剩余空间 */
  overflow-x: hidden; /* 禁止横向滚动 */
}

/* 论坛头部 */
.forum-header {
  margin: 1rem;
  text-align: center;
}

/* 消息框 */
.chat-box {
  background-color: #2c3e50;
  border-radius: 8px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
  flex: 1; /* 让聊天框占满剩余空间 */
  display: flex;
  flex-direction: column;
  padding: 1rem;
  min-height: 0; /* 防止 chat-box 超过父容器 */
}

/* 消息区域样式 */
.messages {
  overflow-y: auto;
  flex: 1; /* 使消息区域可滚动，并占满可用空间 */
  margin-bottom: 16px;
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
  padding: 0 10px; /* 增加左右内边距 */
}

/* 单条消息 */
.message {
  margin-bottom: 10px;
  padding: 12px 16px;
  border-radius: 12px; /* 增加圆角 */
  max-width: 75%; /* 限制消息框的宽度 */
  word-wrap: break-word; /* 防止文本溢出 */
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); /* 增加阴影 */
  transition: all 0.3s ease; /* 动画效果 */
}

/* 自己的消息 */
.my-message {
  background: linear-gradient(145deg, #16a085, #1abc9c); /* 渐变背景 */
  color: white;
  align-self: flex-end; /* 右对齐 */
  border-top-left-radius: 12px; /* 自己的消息左上角圆角 */
  border-bottom-left-radius: 12px; /* 自己的消息左下角圆角 */
  border-top-right-radius: 20px; /* 右上角圆角更大 */
  border-bottom-right-radius: 20px; /* 右下角圆角更大 */
  max-width: 80%; /* 限制消息的最大宽度 */
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); /* 加强阴影效果 */
  margin-left: auto; /* 确保右对齐 */
}

/* 其他人的消息 */
.message:not(.my-message) {
  background-color: #34495e;
  color: #ecf0f1;
  align-self: flex-start; /* 左对齐 */
  border-top-left-radius: 20px; /* 左上角圆角更大 */
  border-bottom-left-radius: 20px; /* 左下角圆角更大 */
  border-top-right-radius: 12px; /* 右上角圆角 */
  border-bottom-right-radius: 12px; /* 右下角圆角 */
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); /* 加强阴影效果 */
}

/* 时间戳 */
.timestamp {
  font-size: 0.75em;
  color: #7f8c8d;
  margin-top: 4px;
  text-align: right;
}

/* 输入框样式 */
.input-box {
  display: flex;
  padding-top: 1rem;
  border-top: 1px solid #eee;
}

input {
  flex: 1;
  padding: 10px;
  border: 1px solid #ccc;
  border-radius: 4px;
  background-color: #ecf0f1;
  color: #34495e;
  font-size: 1rem;
  margin-right: 8px;
  transition: background-color 0.3s ease;
}

input:focus {
  background-color: #f1f1f1;
  outline: none;
}

button {
  padding: 10px 20px;
  border: none;
  background-color: #007bff;
  color: white;
  border-radius: 4px;
  cursor: pointer;
  transition: background-color 0.3s ease;
}

button:hover {
  background-color: #0056b3;
}
</style>