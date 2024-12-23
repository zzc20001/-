<template>
  <div id="mainpage">
    <header>
      <div class="branding">
        <img src="../assets/logo.png" alt="平台 Logo" class="logo" />
        <h1>校园二手物品交易平台</h1>
      </div>
      <div class="search-bar">
        <input type="text" placeholder="搜索您需要的物品..." class="search-input" v-model="searchQuery" />
        <button class="search-button" @click="search">搜索</button>
      </div>
    </header>

<!-- WebSocket 弹窗 -->
<div v-if="showChatWindow" class="chat-window-overlay">
  <div class="chat-container">
    <!-- 聊天窗口头部 -->
    <div class="chat-header">
      <h2>来自{{ sender }}的私聊消息：</h2>
      <button class="btn-close" @click="closeChatWindow">×</button>
    </div>

    <!-- 消息展示区域 -->
    <div class="chat-messages" ref="chatContainer">
      <div 
        v-for="(message, index) in messages" 
        :key="index" 
        :class="{'message-sent': message.sender === currentUser, 'message-received': message.sender !== currentUser}"
        class="chat-message">
        <img 
          v-if="message.sender !== currentUser" 
          :src="require('@/assets/高木.png')" 
          alt="头像" 
          class="message-avatar" />
        <img 
          v-if="message.sender === currentUser" 
          :src="require('@/assets/高木.png')" 
          alt="头像" 
          class="message-avatar" />
        <span>{{message.content }}</span>
        <span class="message-timestamp">{{ formatTimestamp(message.timestamp) }}</span> <!-- 显示时间戳 -->
      </div>
    </div>

    <!-- 输入框区域 -->
    <div class="chat-input-area">
      <input 
        type="text" 
        v-model="newMessage" 
        placeholder="请输入消息..." 
        @keydown.enter="sendMessage" />
      <button @click="sendMessage">发送</button>
    </div>
  </div>
</div>

    <div class="content">
      <aside class="sidebar">
        <div class="profile">
          <img src="../assets/高木.png" alt="用户头像" class="avatar" />
          <h3>祝你所求有所得</h3>
        </div>
        <nav>
          <router-link to="/Mainpage" class="nav-item">返回主页</router-link>
          <router-link to="/profile" class="nav-item">个人中心</router-link>
          <router-link to="/favorites" class="nav-item">收藏夹</router-link>
          <router-link to="/listings" class="nav-item">我的发布</router-link>
          <router-link to="/forum" class="nav-item">论坛</router-link>
        </nav>
      </aside>

      <main class="recommendations">
        <h2>推荐商品</h2>

        <!-- 分类栏 -->
        <div class="category-bar">
          <button 
            v-for="category in categories"
            :key="category"
            :class="{'active': selectedCategory === category}"
            @click="filterByCategory(category)"
          >
            {{ category }}
          </button>
        </div>

        <!-- 商品展示 -->
        <div class="product-grid">
          <router-link
            v-for="item in filteredProducts"
            :key="item.id"
            :to="{ 
              name: 'product-details',
              query: { product: JSON.stringify(item) }
            }"
            class="product-card">
            <img 
              :src="item.img_url
                ? `http://localhost:3000/uploads/${item.img_url.split('/').pop()}` 
                : 'https://via.placeholder.com/200'" 
              alt="商品图片" 
              class="product-image"
            />
            <h3 class="product-title">{{ item.title }}</h3>
            <p class="product-price">{{"商品价格: " + item.price + "元"}}</p>
            <p class="product-description">{{ "商家描述: " + item.desc }}</p>
          </router-link>
        </div>
      </main>
    </div>
  </div>
</template>

<script>
import api from '../api'

export default {
  name: "MainPage",
  data() {
  return {
    // 商品相关数据
    searchQuery: '',
    products: [], // 所有商品数据
    filteredProducts: [], // 根据类别过滤后的商品
    categories: ['全部', '电子产品', '家具', '书籍', '运动用品'], // 假设的商品类别
    selectedCategory: '全部', // 默认显示全部商品
    currentUser: sessionStorage.getItem('username') || 'guest', 
    // 聊天相关数据
    username: '', // 假设当前用户名已经从登录信息中获取
    messages: [],
    showChatWindow: false,
    incomingMessage: '',
    timestamp: '',
    sender: '',
    newMessage:''
  };
},

methods: {
  formatTimestamp(timestamp) {
    const date = new Date(timestamp);
    return `${date.getHours()}:${date.getMinutes()}:${date.getSeconds()}`; // 格式化为 时:分:秒
  },
  sendMessage() {
        if (this.newMessage.trim() !== "") {
          const message = {
            user: this.currentUser, // 当前用户的用户名
            text: this.newMessage,  // 消息内容
            timestamp: new Date().toISOString(), // 当前时间戳
            user_to: this.sender, // 接收者的用户名（如是群聊则为空）
          };

          // 发送消息到服务器
          this.websocket.send(JSON.stringify(message));

          // 添加到本地消息
          this.messages.push({ sender: this.currentUser, content: this.newMessage, timestamp: message.timestamp });
          this.newMessage = "";
          this.scrollToBottom();
        }
      },

  // 显示私聊消息
  showChatMessage(user, text, timestamp) {
    this.incomingMessage = `${text}`;
    this.sender =  `${user}`;
    this.showChatWindow = true; // 显示私聊窗口
    this.timestamp = timestamp; // 保存时间戳
  },

  // 关闭弹窗
  closeChatWindow() {
    this.showChatWindow = false;
  },

  // 滚动到底部（假设你的消息是动态更新的）
  scrollToBottom() {
  this.$nextTick(() => {
    const chatContainer = this.$refs.chatContainer;
    if (chatContainer) {
      chatContainer.scrollTop = chatContainer.scrollHeight;
    } else {
      console.warn("chatContainer 尚未渲染");
    }
  });
},
    async getlistings() {
      try {
        const response = await api.get('/display');
        this.products = response.data.products;
        this.filteredProducts = this.products; // 初始化时展示所有商品
        console.log("获取商品数据成功", this.products);

      } catch (error) {
        console.error("获取商品数据失败", error);
      }
    },
    // 搜索功能
    search() {
      if (this.searchQuery.trim()) {
        this.$router.push({
          path: '/Results',
          query: { query: this.searchQuery.trim() }
        });
      }
    },

    // 根据选中的类别过滤商品
    filterByCategory(category) {
      this.selectedCategory = category;
      if (category === '全部') {
        this.filteredProducts = this.products;
      } else {
        this.filteredProducts = this.products.filter(item => item.category === category);
      }
    }
  },
  mounted() {
    this.getlistings();  // 页面加载时自动调用接口获取商品数据
    this.username = this.currentUser;
    const uid = this.currentUser ? this.currentUser : 0;  // 假设 currentUser 中有 id 字段
    this.websocket = new WebSocket(`ws://localhost:3000/chat?uid=${uid}`);
    
    this.websocket.onopen = () => {
      console.log('WebSocket连接成功');
    };

    this.websocket.onmessage = (event) => {
      console.log('Received message:', event.data);
      
      // 解析收到的消息
      const message = JSON.parse(event.data);

      const { user, text, timestamp, user_to } = message;
      if(user_to){
        this.showChatMessage(user, text, timestamp);
      }
      else{
        console.log('群聊消息被忽略:', message);
      }

      // 将消息添加到消息列表
      this.messages.push({sender: user, content: text, timestamp: timestamp});
      console.log(message.sender);
      //this.scrollToBottom(); // 新消息到来时自动滚动到底部
    };

    this.websocket.onclose = () => {
      console.log('WebSocket连接关闭');
    };
  }
};
</script>


<style scoped>
/* 通用样式 */
#mainpage {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
  font-family: 'Roboto', sans-serif;
  background-color: #f9f9f9; /* 浅灰白背景 */
  color: #333333; /* 深灰色文字 */
}

/* 顶部搜索栏 */
header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1.2rem 2rem;
  background: #ffffff; /* 白色背景 */
  color: #333333;
  border-bottom: 1px solid #dddddd;
}

.branding {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.logo {
  width: 50px;
  height: 50px;
}

.search-bar {
  display: flex;
  align-items: center;
  gap: 0.8rem;
}

.search-input {
  padding: 0.7rem 1rem;
  border-radius: 30px;
  border: 1px solid #cccccc;
  outline: none;
  width: 300px;
  background-color: #ffffff;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
}

.search-button {
  padding: 0.7rem 1.5rem;
  border-radius: 30px;
  background-color: #007bff; /* 蓝色按钮 */
  color: #ffffff;
  font-weight: bold;
  border: none;
  cursor: pointer;
  transition: background-color 0.3s ease;
}

.search-button:hover {
  background-color: #0056b3; /* 深蓝色 */
}

/* 主体部分 */
.content {
  display: flex;
  flex: 1;
  padding: 2rem;
  gap: 2rem;
  justify-content: space-between;
  flex-wrap: wrap;
}

/* 左侧侧边栏 */
.sidebar {
  width: 260px;
  background-color: #ffffff;
  padding: 1.5rem;
  box-shadow: 2px 0 8px rgba(0, 0, 0, 0.1);
  border-radius: 8px;
}

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

.nav-item {
  display: block;
  padding: 0.8rem 1rem;
  margin-bottom: 0.8rem;
  color: #333333;
  text-decoration: none;
  border-radius: 12px;
  transition: background-color 0.3s ease;
}

.nav-item:hover {
  background-color: #f1f1f1;
  color: #000000;
}

/* 推荐区域 */
.recommendations {
  flex: 1;
  padding: 2rem;
  background-color: #ffffff;
  border-radius: 8px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

.category-bar {
  display: flex;
  gap: 1.2rem;
  margin-bottom: 2rem;
  justify-content: flex-start;
}

.category-bar button {
  padding: 0.6rem 1.2rem;
  background-color: #f1f1f1;
  border: 1px solid #cccccc;
  border-radius: 25px;
  cursor: pointer;
  color: #333333;
  transition: all 0.3s ease;
}

.category-bar button.active {
  background-color: #007bff;
  color: white;
}

.product-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
  gap: 1.8rem;
}

.product-card {
  background: #ffffff;
  border-radius: 15px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  padding: 1.2rem;
  text-align: center;
  transition: transform 0.3s ease, box-shadow 0.3s ease;
  cursor: pointer;
  text-decoration: none;
}

.product-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);
}

.product-image {
  width: 100%;
  height: 200px;
  object-fit: cover;
  border-radius: 12px;
  margin-bottom: 1rem;
}

.product-title {
  font-size: 1.3rem;
  margin: 0.5rem 0;
  color: #333333;
}

.product-price {
  font-size: 1.1rem;
  color: #d9534f;
  margin: 0.5rem 0;
}

.product-description {
  font-size: 1rem;
  color: #666666;
  margin-bottom: 1rem;
}

.chat-window-overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.3); /* 半透明灰色背景 */
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
}

/* 聊天容器 */
.chat-container {
  width: 420px;
  height: 600px;
  border-radius: 12px;
  display: flex;
  flex-direction: column;
  background-color: #ffffff;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2);
  overflow: hidden;
  transition: all 0.3s ease;
}

/* 头部 */
.chat-header {
  background-color: #007bff;
  color: white;
  padding: 15px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-size: 18px;
  font-weight: bold;
  border-bottom: 2px solid #dddddd;
}

.chat-header h2 {
  margin: 0;
}

.btn-close {
  background: none;
  border: none;
  color: #ffffff;
  font-size: 20px;
  cursor: pointer;
  transition: color 0.3s ease;
}

.btn-close:hover {
  color: #d9534f; /* 红色 hover 效果 */
}

/* 消息区域 */
.chat-messages {
  flex: 1;
  padding: 20px;
  overflow-y: auto;
  background-color: #f9f9f9;
  display: flex;
  flex-direction: column;
  gap: 15px;
}

.chat-message {
  display: flex;
  align-items: center;
  gap: 10px;
  max-width: 80%;
  word-wrap: break-word;
  padding: 10px;
  border-radius: 10px;
  transition: background-color 0.3s ease;
}

/* 当前用户发送的消息 */
.message-sent {
  align-self: flex-end;
  background-color: #d4edda;
  color: #155724;
}

/* 其他用户发送的消息 */
.message-received {
  align-self: flex-start;
  background-color: #f1f1f1;
  color: #333333;
}

/* 头像 */
.message-avatar {
  width: 40px;
  height: 40px;
  border-radius: 50%;
  object-fit: cover;
  border: 2px solid #ffffff;
  box-shadow: 0 0 5px rgba(0, 0, 0, 0.1);
}

/* 输入框区域 */
.chat-input-area {
  display: flex;
  padding: 10px;
  background-color: #ffffff;
  border-top: 2px solid #dddddd;
}

.chat-input-area input {
  flex: 1;
  padding: 12px;
  font-size: 16px;
  border: 1px solid #cccccc;
  border-radius: 20px;
  outline: none;
  transition: border 0.3s ease;
}

.chat-input-area input:focus {
  border-color: #007bff;
}

.chat-input-area button {
  background-color: #007bff;
  color: white;
  border: none;
  padding: 12px 20px;
  margin-left: 15px;
  border-radius: 20px;
  cursor: pointer;
  font-size: 16px;
  transition: background-color 0.3s ease;
}

.chat-input-area button:hover {
  background-color: #0056b3;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .chat-container {
    width: 100%;
    height: 80%;
  }

  .chat-header h2 {
    font-size: 16px;
  }

  .chat-input-area input {
    font-size: 14px;
  }

  .chat-input-area button {
    font-size: 14px;
  }
}


</style>