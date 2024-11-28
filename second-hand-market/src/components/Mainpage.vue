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

    <div class="content">
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

      <main class="recommendations">
        <h2>推荐商品</h2>
        <div class="product-grid">
          <router-link
            v-for="item in products"
            :key="item.id"
            :to="`/product/${item.id}`"
            class="product-card">
            <img 
              :src="item.img_url
                ? `http://localhost:3000/uploads/${item.img_url.split('/').pop()}` 
                : 'https://via.placeholder.com/200'" 
              alt="商品图片" 
              class="product-image"
            />
            <h3 class="product-title">{{ item.title }}</h3>
            <p class="product-price">￥{{ item.price }}</p>
            <!-- 修改这里，显示商品的描述 -->
            <p class="product-description">{{ item.desc }}</p>
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
        searchQuery: '',
        products: [],
      };
    },
    methods: {
      async getlistings() {
        try {
          const response = await api.get('/display');
          this.products = response.data.products;
          console.log("获取商品数据成功", this.products[0]);
        } catch (error) {
          console.error("获取商品数据失败", error);
        }
      },

      search() {
        if (this.searchQuery.trim()) {
          this.$router.push({
            path: '/Results',
            query: { query: this.searchQuery.trim() }
          });
        }
      }
    },
    mounted() {
      this.getlistings();  // 页面加载时自动调用接口获取商品数据
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
    transition: transform 0.3s ease;
    text-decoration: none;
  }

  /* 图片适应容器 */
  .product-image {
    width: 100%;
    height: 200px;
    object-fit: cover;  /* 确保图片不会超出并且比例保持 */
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

  .product-card:hover {
    transform: scale(1.05);
  }
  
</style>
