<template>
    <div id="profile-page">
      <!-- 顶部用户信息 -->
      <header class="profile-header">
        <div class="profile-info">
          <img src="../assets/高木.png" alt="用户头像" class="avatar" />
          <div class="user-details">
            <h2>{{ item.username || "加载中..." }}</h2>
            <p>邮箱：{{ item.email || "加载中..." }}</p>
            <p>注册日期：{{ item.date || "加载中..." }}</p>
          </div>
          <button class="btn edit-button">编辑资料</button>
        </div>
      </header>
  
      <!-- 主体内容 -->
      <main class="profile-content">
        <!-- 左侧导航栏 -->
        <aside class="sidebar">
          <nav>
            <ul>
              <li>
                <router-link to="/favorites">
                  <i class="icon">&#9733;</i> 我的收藏
                </router-link>
              </li>
              <li>
                <router-link to="/listings">
                  <i class="icon">&#128221;</i> 我的发布
                </router-link>
              </li>
              <li>
                <router-link to="/account-security">
                  <i class="icon">&#128274;</i> 账户安全
                </router-link>
              </li>
            </ul>
          </nav>
        </aside>
  
        <!-- 右侧动态/历史记录 -->
        <section class="history">
          <h3>最近活动</h3>
          <ul class="history-list">
            <li>成功出售《高数教材》 - 2024-11-20</li>
            <li>收藏了商品“二手耳机” - 2024-11-18</li>
            <li>修改了个人信息 - 2024-11-15</li>
          </ul>
        </section>
      </main>
    </div>
  </template>
  
  <script>
  import api from "../api";
  export default {
    name: "ProfilePage",
    data() {
      return {
        item: {},
      };
    },
    mounted() {
    // 挂载时调用 fetchData
    this.fetchData();
    },
    methods: {
      async fetchData() {
        console.log("fetchData 方法被调用");
        try {
          const token = sessionStorage.getItem("token");
          // 使用 API 获取用户信息
          const response = await api.get("/user",{headers: { Authorization: `Bearer ${token}` }});
          if (response.status === 200) {
            const parsedData = JSON.parse(response.data.data);//解析嵌套的json数据
            this.item = {
              username: parsedData.username,
              email: parsedData.email,
              date: parsedData.date,
            };
            console.log("获取用户信息成功", this.item);
          } else {
            console.error("获取用户信息失败", response.status);
          }
        }catch (error) {
          console.error("获取用户信息失败", error);
        }
    },
  }
  };
  </script>
  
  <style scoped>
  /* 页面整体布局 */
  #profile-page {
    display: flex;
    flex-direction: column;
    min-height: 100vh;
    font-family: 'Poppins', sans-serif;
    background: #f9f9f9;
    color: #333;
  }
  
  /* 顶部用户信息 */
  .profile-header {
    background: linear-gradient(135deg, #c3e5e7, #f1e6de);
    padding: 2rem;
    display: flex;
    justify-content: center;
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
  }
  
  .profile-info {
    display: flex;
    align-items: center;
    gap: 1.5rem;
  }
  
  .avatar {
    width: 100px;
    height: 100px;
    border-radius: 50%;
    border: 3px solid #fff;
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
  }
  
  .user-details h2 {
    margin: 0;
    font-size: 1.8rem;
    color: #2c3e50;
  }
  
  .user-details p {
    margin: 0.2rem 0;
    color: #7f8c8d;
  }
  
  .edit-button {
    background: #3498db;
    color: white;
    padding: 0.6rem 1.2rem;
    border: none;
    border-radius: 20px;
    cursor: pointer;
    transition: background 0.3s;
  }
  
  .edit-button:hover {
    background: #2980b9;
  }
  
  /* 主体内容布局 */
  .profile-content {
    display: flex;
    padding: 2rem;
    gap: 2rem;
  }
  
  /* 左侧导航栏 */
  .sidebar {
    width: 20%;
    background: white;
    padding: 1.5rem;
    border-radius: 10px;
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.05);
  }
  
  .sidebar ul {
    list-style: none;
    padding: 0;
    margin: 0;
  }
  
  .sidebar li {
    margin-bottom: 1rem;
  }
  
  .sidebar a {
    text-decoration: none;
    color: #2c3e50;
    font-weight: bold;
    display: flex;
    align-items: center;
    gap: 0.5rem;
    transition: color 0.3s;
  }
  
  .sidebar a:hover {
    color: #3498db;
  }
  
  .icon {
    font-size: 1.2rem;
  }
  
  /* 动态/历史记录 */
  .history {
    flex: 1;
    background: white;
    padding: 1.5rem;
    border-radius: 10px;
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.05);
  }
  
  .history h3 {
    font-size: 1.5rem;
    margin-bottom: 1rem;
    color: #2c3e50;
  }
  
  .history-list {
    list-style: none;
    padding: 0;
  }
  
  .history-list li {
    margin-bottom: 0.8rem;
    padding: 0.8rem;
    border-left: 4px solid #3498db;
    background: #f9f9f9;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.05);
    border-radius: 5px;
  }
  </style>
  