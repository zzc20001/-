<template>
    <div id="mainpage">
      <!-- 顶部搜索栏 -->
      <header>
        <div class="branding">
          <img src="../assets/logo.png" alt="平台 Logo" class="logo" />
          <h1>校园二手物品交易平台</h1>
        </div>
        <div class="search-bar">
          <input type="text" placeholder="搜索您需要的物品..." class="search-input" v-model="searchQuery"  />
          <button class="search-button"  @click="search">搜索</button>
        </div>
      </header>
  
      <!-- 主体部分 -->
      <div class="content">
        <!-- 左侧侧边栏 -->
        <aside class="sidebar">
          <div class="profile">
            <img src="../assets/高木.png" alt="用户头像" class="avatar" />
            <h3>祝你所求有所得</h3>
          </div>
          <nav>
            <router-link to="/profile" class="nav-item">个人中心</router-link>
            <router-link to="/favorites" class="nav-item">收藏夹</router-link>
            <router-link to="/listings" class="nav-item">我的发布</router-link>
          </nav>
        </aside>
  
        <!-- 推荐区域 -->
        <main class="recommendations">
          <h2>推荐商品</h2>
          <div class="product-grid">
            <div class="product-card" v-for="item in products" :key="item.id">
              <img :src="item.image" alt="商品图片" class="product-image" />
              <h3 class="product-title">{{ item.title }}</h3>
              <p class="product-price">￥{{ item.price }}</p>
              <p class="product-description">{{ item.description }}</p>
            </div>
          </div>
        </main>
      </div>
    </div>
  </template>
  
  <script>
  export default {
    name: "MainPage",
    data() {
      return {
        searchQuery: '',
        products: [
          {
            id: 1,
            title: "二手台灯",
            price: 50,
            description: "女生自用九九新，可调光。",
            image: "../assets/logo.png",
          },
          {
            id: 2,
            title: "二手课本",
            price: 30,
            description: "适合大一使用，含学姐笔记。",
            image: "../assets/logo.png",
          },
        ],
      };
    },
    methods: {
      search(){
        if(this.searchQuery.trim()){
          this.$router.push({
            path:'/Results',
            query: {query : this.searchQuery.trim()}
          })
        }
      }
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
  }
  
  /* 顶部搜索栏 */
  header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 1rem 2rem;
    background: linear-gradient(135deg, #4a90e2, #50e3c2);
    color: white;
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
    gap: 0.5rem;
  }
  
  .search-input {
    padding: 0.5rem 1rem;
    border-radius: 20px;
    border: none;
    outline: none;
    width: 300px;
  }
  
  .search-button {
    padding: 0.5rem 1.5rem;
    border-radius: 20px;
    background-color: #ffffff;
    color: #4a90e2;
    font-weight: bold;
    border: none;
    cursor: pointer;
  }
  
  .search-button:hover {
    background-color: #d4f4ff;
  }
  
  /* 主体部分 */
  .content {
    display: flex;
    flex: 1;
  }
  
  /* 左侧侧边栏 */
  .sidebar {
    width: 250px;
    background-color: #f7f9fc;
    padding: 1rem;
    box-shadow: 2px 0 5px rgba(0, 0, 0, 0.1);
  }
  
  .profile {
    text-align: center;
    margin-bottom: 1.5rem;
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
    margin-bottom: 0.5rem;
    color: #333;
    text-decoration: none;
    border-radius: 10px;
  }
  
  .nav-item:hover {
    background-color: #e6f2ff;
    color: #007bff;
  }
  
  /* 推荐区域 */
  .recommendations {
    flex: 1;
    padding: 2rem;
    background-color: #f0f2f5;
  }
  
  .product-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 1.5rem;
  }
  
  .product-card {
    background: white;
    border-radius: 10px;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
    padding: 1rem;
    text-align: center;
  }
  
  .product-image {
    width: 100%;
    border-radius: 10px;
    margin-bottom: 0.5rem;
  }
  
  .product-title {
    font-size: 1.2rem;
    margin: 0.5rem 0;
    color: #333;
  }
  
  .product-price {
    font-size: 1rem;
    color: #e74c3c;
    margin: 0.5rem 0;
  }
  
  .product-description {
    font-size: 0.9rem;
    color: #7f8c8d;
  }
  </style>
  