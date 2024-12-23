  <template>
    <div class="product-details">
      <div v-if="product" class="product-card">
        <div class="product-image-container">
          <img 
            :src="product.img_url
              ? `http://localhost:3000/uploads/${product.img_url.split('/').pop()}` 
              : 'https://via.placeholder.com/200'"
            alt="商品图片"
            class="product-image" />
        </div>
        <div class="product-info">
          <h1 class="product-title">{{ product.title }}</h1>
          <p class="product-price"><strong>价格:</strong> {{ product.price }} 元</p>
          <p class="product-category"><strong>类别:</strong> {{ product.category }}</p>
          <p class="product-desc"><strong>描述:</strong> {{ product.desc }}</p>
          <div class="action-buttons">
            <button @click="addToFavorites" class="btn btn-favorites">添加到收藏夹</button>
            <button @click="openChatWindow" class="btn btn-contact">联系卖家</button>
            <button @click="buyit" class="btn btn-buy">购买</button>
          </div>
        </div>
      </div>
      <div v-else>
        <p>商品加载中...</p>
      </div>
      <div v-if="isPurchaseModalOpen" class="purchase-modal-overlay">
      <div class="purchase-modal-container">
        <h2>请选择付款状态</h2>
        <div class="purchase-modal-buttons">
          <button @click="confirmPayment(true)" class="btn btn-paid">已付款</button>
          <button @click="confirmPayment(false)" class="btn btn-unpaid">未付款</button>
        </div>
        <button @click="closePurchaseModal" class="btn-close">×</button>
      </div>
    </div>
      <!-- 聊天窗口 -->
      <div v-if="isChatOpen" class="chat-window-overlay">
        <div class="chat-container">
          <!-- 聊天窗口头部 -->
          <div class="chat-header">                                     
            <h2>{{ chatTitle }}</h2>
            <button class="btn-close" @click="closeChatWindow">×</button>
          </div>

          <!-- 消息展示区域 -->
          <div class="chat-messages" ref="messageContainer">
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
              <span>{{ message.content }}</span>
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
    </div>
  </template>

  <script>
  import api from "../api";
  export default {
    data() {
      return {
        product: this.$route.query.product ? JSON.parse(this.$route.query.product) : null,
        currentUser: sessionStorage.getItem('username') || 'guest', // 当前用户，假设保存在sessionStorage
        chatTitle: '', // 聊天对象的名字
        messages: [], // 消息列表
        newMessage: "",
        isChatOpen: false,
        isPurchaseModalOpen: false, 
        socket: null, // WebSocket 连接对象
      };
    },
    mounted() {
      // 初始化WebSocket连接
      if (this.product) {
        this.chatTitle = this.product.usr_id; // 通过 product 初始化 chatTitle
      }
      const uid = this.currentUser ? this.currentUser : 0;  // 假设 currentUser 中有 id 字段
      this.socket = new WebSocket(`ws://localhost:3000/chat?uid=${uid}`);
      
      this.socket.onopen = () => {
        console.log('WebSocket connection established');
      };

      this.socket.onmessage = (event) => {
        const message = JSON.parse(event.data); // 假设后端发送的是JSON格式的消息
        const { user, text, timestamp, user_to} = message;
        if(user_to !== this.currentUser && user !== this.currentUser) return; // 如果不是发给当前用户的消息则忽略
        this.messages.push({sender: user, content : text, timestamp: timestamp});  // 接收到消息后，加入消息列表
        this.scrollToBottom(); // 自动滚动到底部
      };

      this.socket.onclose = () => {
        console.log('WebSocket connection closed');
      };

      this.socket.onerror = (error) => {
        console.error('WebSocket error:', error);
      };
    },
    methods: {
      formatTimestamp(timestamp) {
    const date = new Date(timestamp);
    return `${date.getHours()}:${date.getMinutes()}:${date.getSeconds()}`; // 格式化为 时:分:秒
    },
      openChatWindow() {
        this.isChatOpen = true;
        console.log("打开聊天窗口");
      },
      closeChatWindow() {
        this.isChatOpen = false;
        console.log("关闭聊天窗口");
      },
      closePurchaseModal() {
      this.isPurchaseModalOpen = false;
    },
      buyit(){
        this.isPurchaseModalOpen = true;
      },
      confirmPayment(paymentStatus) {
      // 确认付款状态
      const purchaseData = {
        buyer_name: this.currentUser, // 当前用户 ID
        seller_id: this.product.usr_id, // 商品所有者 ID
        product_id: this.product.id, // 商品 ID
        is_pay: paymentStatus, // 付款状态
      };

      api.post("/buy", purchaseData)
        .then(() => {
          if(paymentStatus)
          alert("商品购买成功！");
          else alert ("商品未付款！");
          this.isPurchaseModalOpen = false;
        });
    },
      sendMessage() {
        if (this.newMessage.trim() !== "") {
          const message = {
            user: this.currentUser, // 当前用户的用户名
            text: this.newMessage,  // 消息内容
            timestamp: new Date().toISOString(), // 当前时间戳
            user_to: this.chatTitle, // 接收者的用户名（如是群聊则为空）
          };

          // 发送消息到服务器
          this.socket.send(JSON.stringify(message));

          // 添加到本地消息
          this.messages.push({ sender: this.currentUser, content: this.newMessage , timestamp: message.timestamp});
          this.newMessage = "";
          this.scrollToBottom();
        }
      },
      scrollToBottom() {
        this.$nextTick(() => {
        const container = this.$refs.messageContainer;
        if (container) {
            container.scrollTop = container.scrollHeight;
        }
        });
      },
    },
  };
  </script>

  <style scoped>
  /* 聊天样式保持不变 */
  .chat-window-overlay {
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background-color: rgba(0, 0, 0, 0.5);
    display: flex;
    justify-content: center;
    align-items: center;
    z-index: 1000;
  }

  .chat-container {
    width: 400px;
    background: #fff;
    border-radius: 12px;
    overflow: hidden;
    display: flex;
    flex-direction: column;
  }

  .chat-header {
    padding: 1rem;
    background: #2ecc71;
    color: #fff;
    display: flex;
    justify-content: space-between;
    align-items: center;
  }

  .chat-messages {
    flex: 1;
    padding: 1rem;
    overflow-y: auto;
    background: #f9f9f9;
  }

  .chat-message {
    margin: 0.5rem 0;
    padding: 0.5rem;
    border-radius: 8px;
    max-width: 80%;
  }

  .message-sent {
    align-self: flex-end;
    background: #2ecc71;
    color: white;
  }

  .message-received {
    align-self: flex-start;
    background: #ecf0f1;
    color: #2c3e50;
  }

  .chat-input {
    display: flex;
    padding: 0.5rem;
    background: #f1f1f1;
  }

  .chat-input input {
    flex: 1;
    padding: 0.5rem;
    border: 1px solid #ddd;
    border-radius: 4px;
  }

  .chat-input .btn-send {
    margin-left: 0.5rem;
    background: #2ecc71;
    color: white;
    border: none;
    padding: 0.5rem 1rem;
    border-radius: 4px;
    cursor: pointer;
  }

  .product-details {
    padding: 3rem;
    background-color: #f1f1f1;
    min-height: 100vh;
  }

  .product-card {
    display: flex;
    background-color: #fff;
    border-radius: 12px;
    box-shadow: 0 8px 24px rgba(0, 0, 0, 0.15);
    overflow: hidden;
    max-width: 1200px;
    margin: auto;
    padding: 2rem;
  }
    .product-image-container {
      flex: 1;
      max-width: 400px;
      margin-right: 2rem;
    }
    
    .product-image {
      width: 100%;
      height: auto;
      object-fit: cover;
      border-radius: 12px;
      box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
    }
    
    .product-info {
      flex: 2;
    }
    
    .product-title {
      font-size: 2rem;
      font-weight: bold;
      color: #34495e; /* 深灰蓝色 */
      margin-bottom: 1rem;
    }
    
    .product-price,
    .product-category,
    .product-desc {
      font-size: 1.2rem;
      color: #7f8c8d; /* 暖灰色 */
      margin-bottom: 1rem;
    }
    
    .action-buttons {
      display: flex;
      gap: 1rem;
    }
    
    .btn {
      padding: 0.8rem 1.6rem;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: background-color 0.3s ease;
      font-size: 1rem;
    }
    
    .btn-favorites {
      background-color: #f39c12; /* 温暖的橙色 */
      color: #fff;
    }
    
    .btn-favorites:hover {
      background-color: #e67e22; /* 橙色的深色 */
    }
    
    .btn-contact {
      background-color: #2ecc71; /* 亮绿色 */
      color: #fff;
    }
    
    .btn-contact:hover {
      background-color: #27ae60; /* 绿色的深色 */
    }
    .btn-buy {
      background-color: #FF007F; /* 亮绿色 */
      color: #fff;
    }
    
    .btn-buy:hover {
      background-color: #FF007F; /* 绿色的深色 */
    }
    
    @media (max-width: 768px) {
      .product-card {
        flex-direction: column;
      }
    
      .product-image-container {
        max-width: 100%;
        margin-bottom: 2rem;
      }
    
      .product-info {
        text-align: center;
      }
    
      .action-buttons {
        flex-direction: column;
      }
    }
  .chat-container {
    width: 400px;
    height: 600px;
    border: 1px solid #ddd;
    border-radius: 8px;
    display: flex;
    flex-direction: column;
    background-color: #f9f9f9;
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
    overflow: hidden;
  }

  /* 头部样式 */
  .chat-header {
    background-color: #f5f5f5;
    padding: 10px 15px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    border-bottom: 1px solid #ddd;
  }

  .chat-header h2 {
    font-size: 16px;
    margin: 0;
  }

  .btn-close {
    background: none;
    border: none;
    font-size: 18px;
    cursor: pointer;
    color: #888;
  }
  /* 消息展示区 */
  .chat-messages {
    flex: 1;
    padding: 10px;
    overflow-y: auto;
    background-color: #ffffff;
    display: flex;
    flex-direction: column;
  }

  /* 消息容器 */
  .chat-message {
    display: flex;
    align-items: flex-start; /* 默认左对齐 */
    margin-bottom: 10px;
  }

  /* 当前用户发送的消息 */
  .message-sent {
    justify-content: flex-end;  /* 右对齐 */
  }

  /* 其他用户发送的消息 */
  .message-received {
    justify-content: flex-start; /* 左对齐 */
  }

  /* 头像 */

  .message-avatar {
    width: 40px; /* 设置头像宽度 */
    height: 40px; /* 设置头像高度 */
    border-radius: 50%; /* 使头像圆形 */
    object-fit: cover; /* 保持图片比例填充容器，可能裁剪图片 */
  }
  /* 发送的消息内容样式 */
  .message-sent span {
    background-color: #dcf8c6;
    color: #333;
    padding: 8px 12px;
    border-radius: 8px;
    max-width: 70%;
    word-wrap: break-word;
    display: inline-block;
  }

  /* 接收的消息内容样式 */
  .message-received span {
    background-color: #f1f1f1;
    color: #333;
    padding: 8px 12px;
    border-radius: 8px;
    max-width: 70%;
    word-wrap: break-word;
    display: inline-block;
  }

  /* 输入框区域 */
  .chat-input-area {
    display: flex;
    padding: 10px;
    background-color: #f5f5f5;
    border-top: 1px solid #ddd;
  }

  .chat-input-area input {
    flex: 1;
    padding: 10px;
    border: 1px solid #ddd;
    border-radius: 4px;
    outline: none;
  }

  .chat-input-area button {
    margin-left: 10px;
    padding: 10px 20px;
    background-color: #4caf50;
    color: #fff;
    border: none;
    border-radius: 4px;
    cursor: pointer;
  }
  .purchase-modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.5);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
}

.purchase-modal-container {
  background-color: #fff;
  padding: 20px;
  border-radius: 10px;
  display: flex;
  flex-direction: column;
  align-items: center;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
}

.purchase-modal-buttons {
  display: flex;
  gap: 20px;
  margin-top: 20px;
}

.purchase-modal-container button {
  padding: 10px 20px;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  font-size: 16px;
}

.btn-paid {
  background-color: #2ecc71;
  color: white;
}

.btn-unpaid {
  background-color: #e74c3c;
  color: white;
}

.btn-close {
  background: none;
  border: none;
  font-size: 24px;
  cursor: pointer;
  color: #888;
  position: absolute;
  top: 10px;
  right: 10px;
}
</style>
    