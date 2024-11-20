<template>
    <div class="register-container">
      <h2>注册</h2>
      <form @submit.prevent="handleRegister">
        <div>
          <label for="username">用户名:</label>
          <input type="text" id="username" v-model="username" required />
        </div>
        <div>
          <label for="password">密码:</label>
          <input type="password" id="password" v-model="password" required />
        </div>
        <button type="submit">注册</button>
      </form>
    </div>
  </template>
  
  <script>
  export default {
    name : "UserRegister",
    data() {
      return {
        username: "",
        password: "",
      };
    },
    methods: {
      async handleRegister() {
        try {
          const response = await fetch("http://localhost:5000/register", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
              username: this.username,
              password: this.password,
            }),
          });
          if (response.ok) {
            alert("注册成功！");
          } else {
            alert("注册失败，请重试！");
          }
        } catch (error) {
          console.error("注册错误:", error);
        }
      },
    },
  };
  </script>
  
  <style scoped>
  .register-container {
    max-width: 400px;
    margin: 0 auto;
    padding: 1rem;
    border: 1px solid #ccc;
    border-radius: 5px;
  }
  </style>
  