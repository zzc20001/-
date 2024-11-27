<template>
    <div class="search-page">
      <!-- 搜索框 -->
      <div class="search-bar">
        <input 
          type="text" 
          v-model="searchQuery" 
          @keyup.enter="search" 
          placeholder="搜索您需要的物品..." 
          class="search-input"
        />
        <button @click="search" class="search-button">搜索</button>
      </div>
  
      <!-- 搜索结果 -->
      <div v-if="isLoading" class="loading">
        <span>加载中...</span>
      </div>
  
      <div v-else-if="results.length === 0" class="no-results">
        <span>没有找到相关内容。</span>
      </div>
  
      <div v-else class="results">
        <div v-for="product in results" :key="product.id" class="product-card">
          <img :src="product.image_path 
              ? `http://localhost:3000/uploads/${product.image_path.split('/').pop()}` 
              : 'https://via.placeholder.com/200'" 
            alt="商品图片" />
          <div class="product-details">
            <h3>{{ product.title }}</h3>
            <p class="product-description">{{ product.description }}</p>
            <p class="product-price">价格: ¥{{ product.price }}</p>
            <button @click="viewProduct(product.id)" class="view-button">查看详情</button>
          </div>
        </div>
      </div>
  
      <!-- 分页 -->
      <div v-if="totalPages > 1" class="pagination">
        <button @click="goToPage(currentPage - 1)" :disabled="currentPage === 1">上一页</button>
        <span>{{ currentPage }} / {{ totalPages }}</span>
        <button @click="goToPage(currentPage + 1)" :disabled="currentPage === totalPages">下一页</button>
      </div>
    </div>
  </template>
  
  <script>
  import api from "../api";
  
  export default {
    name: 'SearchResults',
    data() {
      return {
        searchQuery: this.$route.query.query || '',
        results: [],
        isLoading: false,
        currentPage: 1,
        totalPages: 1,
      };
    },
    mounted() {
      console.log(this.searchQuery);
      this.search();
    },
    methods: {
      async search() {
        if (!this.searchQuery.trim()) return;
  
        this.isLoading = true;
        this.results = [];
  
        try {
          const response = await api.get("/search", {
            params: {
              query: this.searchQuery.trim(), // 发送查询参数
            },
          });
  
          // 处理响应数据
          if (response.data.status === "success") {
            const allResults = response.data.data; // 获取搜索到的产品数据
            this.totalPages = Math.ceil(allResults.length / 10); // 假设每页最多显示 10 个产品
            this.results = allResults.slice((this.currentPage - 1) * 10, this.currentPage * 10); // 当前页的数据
          } else {
            console.error("Error:", response.data.message);
          }
        } catch (error) {
          console.error(error);
        }
  
        this.isLoading = false;
      },
      viewProduct(productId) {
        // 跳转到产品详情页，假设使用 Vue Router
        this.$router.push({ path:'/product-detail',name: 'product-detail', params: { id: productId } });
      },
      goToPage(page) {
        if (page < 1 || page > this.totalPages) return;
        this.currentPage = page;
        this.search(); // 重新获取数据，加载当前页的数据
      },
    }
  };
  </script>
  
  
  <style scoped>
  .search-page {
    max-width: 1200px;
    margin: 0 auto;
    padding: 20px;
    background: linear-gradient(to right, #f0f8ff, #e6f7ff); /* 漸變背景 */
    border-radius: 12px; /* 圆角效果 */
  }
  
  .search-bar {
    display: flex;
    justify-content: center;
    margin-bottom: 20px;
  }
  
  .search-input {
    padding: 12px;
    font-size: 16px;
    width: 350px;
    margin-right: 15px;
    border: 1px solid #ccc;
    border-radius: 25px; /* 圆角 */
    background-color: #fff;
    transition: border-color 0.3s ease; /* 过渡效果 */
  }
  
  .search-input:focus {
    border-color: #007BFF; /* 聚焦时的边框颜色 */
  }
  
  .search-button {
    padding: 12px 25px;
    font-size: 16px;
    background-color: #ff6f00; /* 橙色 */
    color: white;
    border: none;
    border-radius: 25px; /* 圆角 */
    cursor: pointer;
    transition: background-color 0.3s ease;
  }
  
  .search-button:hover {
    background-color: #0056b3; /* 鼠标悬停时的颜色 */
  }
  
  .loading {
    text-align: center;
    font-size: 18px;
  }
  
  .no-results {
    text-align: center;
    font-size: 18px;
    color: #888;
  }
  
  .results {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
    gap: 20px;
  }
  
  .product-card {
  background: #fff;
  border-radius: 12px;
  box-shadow: 0 6px 12px rgba(0, 0, 0, 0.1);
  padding: 20px;
  display: flex;
  flex-direction: column;
  align-items: center;
  text-align: center;
  transition: transform 0.3s ease, box-shadow 0.3s ease;
}
  
  .product-card:hover {
    transform: translateY(-5px); /* 鼠标悬停时的升高效果 */
    box-shadow: 0 8px 16px rgba(0, 0, 0, 0.15); /* 增强阴影效果 */
  }
  
  .product-card img {
  width: 100%;
  height: 120px;
  object-fit: cover;
  border-radius: 10px;
  margin: 10px 0;
}

  
  .product-details {
    margin-top: 15px;
    text-align: center;
  }
  
  .product-description {
    font-size: 14px;
    color: #555;
  }
  
  .product-price {
    font-size: 16px;
    color: #007BFF;
    margin: 15px 0;
  }
  
  .view-button {
    padding: 10px 20px;
    background-color: #28a745;
    color: white;
    border: none;
    border-radius: 25px;
    cursor: pointer;
    transition: background-color 0.3s ease;
  }
  
  .view-button:hover {
    background-color: #218838;
  }
  
  .pagination {
    text-align: center;
    margin-top: 20px;
  }
  
  .pagination button {
    padding: 10px 20px;
    font-size: 16px;
    background-color: #f1f1f1;
    border: 1px solid #ddd;
    cursor: pointer;
    margin: 0 10px;
    border-radius: 25px;
    transition: background-color 0.3s ease;
  }
  
  .pagination button:disabled {
    background-color: #ccc;
    cursor: not-allowed;
  }
  
  .pagination button:hover:not(:disabled) {
    background-color: #007BFF;
    color: white;
  }
  </style>
  